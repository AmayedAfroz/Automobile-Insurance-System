# Audit Part 2 — Discrepancies, Risks & Action Plan

---

## 1. Discrepancies & Missing Features

### DISC-01 — `Rejected` Claim Status Is Declared But Never Implemented
**Severity: Medium**

`Claim.h` documents "Pending", "Approved", "Rejected" as valid statuses in a comment. The `updateStatus()` method exists. However:

- `ClaimController` has **no `rejectClaim()` method**.
- The Manager tab in `UIManager.cpp` has **no Reject button**.
- If a manager examines an inspection report and decides the claim is fraudulent or ineligible, there is no way to record that decision.

This is a **prompt gap**: the prompt says "Manager gives approval for a claim after examining the inspection report" — by implication, the ability to reject is also a required use case.

---

### DISC-02 — No Dedicated "View All Inspection Reports" for Manager
**Severity: Medium**

The prompt explicitly states:
> *"A manager can view/generate various reports such as... inspection reports"*

`reports.txt` is written correctly and data persists. However:
- There is no `fetchAllInspectionReports()` method in `ClaimController`.
- The Manager tab has no "Fetch Inspection Reports" button.
- The only way the manager indirectly interacts with reports is through `approveClaim`, which checks internally for the report's existence but never displays it.

---

### DISC-03 — No Workshop Management Use Case
**Severity: Medium**

The prompt says: *"The company has many registered workshops."*

`Workshop.h` exists as an entity class. `workshops.txt` is read during validation. However:
- There is **no method to register a new workshop** at runtime.
- There is **no method to list all registered workshops**.
- The `workshops.txt` file must be **manually pre-populated** before the system is usable, which is an invisible dependency that will fail during a live demo.
- `Workshop.cpp` is absent from the source — `Workshop::Workshop()`, `getWorkshopID()`, `getName()`, and `verifyRegistration()` have **no implementation file**, meaning `Workshop.h` cannot be compiled independently.

---

### DISC-04 — `fetchNewCustomersReport` Has No Date/Month Filtering
**Severity: Low-Medium**

The prompt states:
> *"list of new customers won in a month"*

`UserController::fetchNewCustomersReport()` returns **all customers ever registered**, with no filtering by month or date range. The `registrationDate` field is stored in `users.txt` but is never read or compared during the report. This means on a large dataset the manager gets a report of all historical customers, not just the current month's acquisitions.

---

### DISC-05 — `generateID` Race Condition Under Same-Second Calls
**Severity: Low (academic context)**

```cpp
long long ts = static_cast<long long>(std::time(nullptr)); // 1-second resolution
int rnd = std::rand() % 9000 + 1000;
```

`std::time()` has 1-second resolution. If two records are created within the same second (e.g., quickly registering a vehicle then immediately issuing a policy during a demo), and `std::rand()` produces the same value (it is seeded once at startup in `main` with `std::srand`), the two generated IDs will be **identical**, causing a data collision in the flat file. `std::srand` is called once but the RNG state is shared between `UserController` and `ClaimController`, which are separate instances.

---

### DISC-06 — `Vehicle.h` Missing `ownerID` Field
**Severity: Medium**

The `Vehicle` entity class has:
```cpp
std::string vehicleID;
std::string make;
std::string licensePlate;
```

It has **no `ownerID` field**. The ownership link exists only in the CSV record in `vehicles.txt` (column 2), but the domain model class does not represent it. This is a semantic gap: `Vehicle` as an OOP entity has no knowledge of who owns it. A professor reviewing the class diagram will notice this immediately.

---

### DISC-07 — Entity Classes Are Not Used at Runtime
**Severity: Low (design observation)**

`Customer`, `Vehicle`, `Claim`, `InspectionReport`, and `InsurancePolicy` are declared as proper OOP classes with constructors and getters, but they are **never instantiated** during actual system operation. All data flows as raw `std::string` CSV lines between the controllers and FileHandler. The entity classes serve only as documentation of the data model.

While this is an acceptable pragmatic design for a file-based system, a professor may ask *"why define these classes if you never create objects from them?"* A simple and defensible answer requires one example of an entity object being constructed during a display operation (e.g., parsing a CSV line into a `Claim` object before displaying it).

---

## 2. Reliability & Crash Risks

### RISK-01 — `workshops.txt` Must Exist and Be Pre-Populated Before Any Report Can Be Submitted
**Impact: Demo-breaking**

`ClaimController::validateWorkshopRegistration` reads `workshops.txt`. If this file does not exist or is empty, `loadRecords` correctly returns an empty vector and the function returns `false` — meaning **every single inspection report submission will be rejected**, silently, with the message "Workshop not registered." There is no UI indication that the data file itself is missing.

**Fix:** Add a "Register Workshop" section to the Manager or a dedicated Admin tab, or seed a `workshops.txt` file at startup if it doesn't exist.

---

### RISK-02 — CSV Fields Containing Commas Will Corrupt All Records
**Impact: Data corruption**

The CSV parser uses simple `std::getline(ss, field, ',')`. Any user input containing a comma (e.g., address `"123 Main St, Apt 4"`, or a description `"Front bumper damaged, hood dented"`) will split into extra fields, misaligning all subsequent column reads. This affects:
- `registerCustomer` (address field)
- `fileClaim` (description field)
- `submitInspectionReport` (implicit)

**Fix:** Either strip commas from input before saving, use a different delimiter (`|` or `\t`), or wrap fields in quotes with a proper CSV parser.

---

### RISK-03 — `approveClaim` Can Silently Fail If Claim Was Already Approved
**Impact: Confusing UX**

`FileHandler::updateRecord` searches for the token `,Pending,` in the claim line. If the claim is already Approved (status was already flipped), the token won't be found, `updated` stays false, and the function returns `false`. The UI then shows `[Error] Not found or no inspection report exists.` — which is the **wrong error message**. The manager has no way to know whether the failure was "claim doesn't exist", "no report exists", or "already approved."

---

### RISK-04 — `generateID` Is Not Seeded in Both Controllers
**Impact: Potential ID collision**

`std::srand` is only called once in `AutoInsuranceSystem.cpp::main()`. However, `UserController` and `ClaimController` are separate object instances, each with their own `generateID()`. If both generate an ID within the same second, the output of `std::rand()` could collide since the PRNG state is global and shared. On a fast machine during a demo this is a realistic risk.

---

### RISK-05 — No Input Sanitization on `strlen` Validation in UI
**Impact: Whitespace bypass**

The UI validation guard is:
```cpp
bool custBlank = (strlen(regName) == 0 || strlen(regPass) == 0);
```
A user can enter a single space `" "` and pass this check, resulting in a customer record with `name=" "` saved to file. This will not crash the system but produces junk data that breaks the customer report and claim history join.

---

### RISK-06 — `updateRecord` Uses String Replacement, Not Field-Aware Parsing
**Impact: Subtle corruption on edge cases**

```cpp
line.replace(pos, oldValue.length(), newValue);
```
`oldValue` is `,Pending,`. If a claim description happens to contain the literal substring `,Pending,` (e.g., `"Settlement is Pending, awaiting workshop"`), the first occurrence found by `line.find(matchToken)` will be in the correct line (due to `matchToken = claimID`), but `line.find(oldValue)` could match the description text instead of the status field. The status would be left unchanged and the description would be corrupted.

---

## 3. Prioritized Action Plan

### Priority 1 — Demo-Breaking Issues (Fix Before Submission)

- **[P1-A] Seed `workshops.txt` at startup.**
  In `AutoInsuranceSystem.cpp::main()`, after `UIManager.init()`, check if `workshops.txt` exists; if not, write 2–3 sample registered workshops so the surveyor workflow functions immediately without manual file creation.

- **[P1-B] Implement `Workshop.cpp`.**
  `Workshop.h` declares a constructor and three methods with no corresponding `.cpp`. This will cause a linker error if any translation unit ever constructs a `Workshop` object. Create `Workshop.cpp` implementing the declared methods.

- **[P1-C] Add a "Register Workshop" form to the Manager tab.**
  Without this, the system requires a text editor to function. A simple form (Workshop ID, Name) writing `ID,Name,1` to `workshops.txt` makes the system self-contained.

### Priority 2 — Prompt Compliance Gaps (Required for Full Marks)

- **[P2-A] Add `rejectClaim(string claimID)` to `ClaimController`.**
  Flip status from `Pending` to `Rejected` using `updateRecord`. Add a "Reject" button next to "Approve" in the Manager tab. This fulfils the implied rejection use case.

- **[P2-B] Add `fetchAllInspectionReports()` to `ClaimController`.**
  Return all lines from `reports.txt`. Add a "Fetch Inspection Reports" button to the Manager tab. This directly satisfies the prompt's *"inspection reports"* report requirement.

- **[P2-C] Add date filtering to `fetchNewCustomersReport()`.**
  Parse the date field (column 6) from each user record and filter by current month/year using `std::tm`. Without this, the "new customers in a month" report is factually incorrect.

- **[P2-D] Add `ownerID` field to `Vehicle.h`.**
  The class diagram submitted for the project must show Vehicle with an ownership association to Customer. Adding `std::string ownerID` to the class and its constructor makes the domain model accurate.

### Priority 3 — Reliability Hardening (Strongly Recommended)

- **[P3-A] Replace commas with `|` as the CSV delimiter** in all `saveRecord` calls and all `getline(ss, field, ',')` parsers. This eliminates RISK-02 entirely with minimal code change.

- **[P3-B] Improve `approveClaim` error reporting.**
  Return an enum or an error code (`enum class ApproveResult { Success, NoReport, NotFound, AlreadyApproved }`) instead of a plain `bool`, so the UI can display the correct diagnostic message.

- **[P3-C] Trim whitespace in UI validation.**
  Before the `strlen` check, scan `regName` for non-space characters. A simple helper `bool isBlankOrWhitespace(const char* s)` prevents junk records.

- **[P3-D] Add `std::srand` call in `main` and guard `generateID` against same-second collisions.**
  Use `std::chrono::high_resolution_clock` for a nanosecond-resolution timestamp instead of `std::time()`, or use a static counter that increments with each call. This eliminates RISK-04.

### Priority 4 — Academic Polish (Optional, Boosts Grade)

- **[P4-A] Instantiate entity objects during display.**
  In `renderManagerTab`, when displaying pending claims, parse each CSV line into a `Claim` object and display its fields individually (ClaimID, Status, Description) instead of dumping the raw CSV string. This demonstrates the entity classes have runtime purpose.

- **[P4-B] Add a `\r\n` strip to `loadRecords`.**
  On Windows, files written with `\n` may leave `\r` at the end of each line when read back on certain toolchains. Trim trailing `\r` from each line in `loadRecords` to prevent token-matching failures (e.g., `isRegistered == "1"` failing because the field is actually `"1\r"`).

- **[P4-C] Document the class diagram and use case diagram references.**
  The prompt requires a class diagram and use case diagram alongside the code. Ensure the diagrams match the current 14-class architecture including both controllers, all 5 entity-role classes, Vehicle, InsurancePolicy, Claim, InspectionReport, Workshop, and FileHandler.
