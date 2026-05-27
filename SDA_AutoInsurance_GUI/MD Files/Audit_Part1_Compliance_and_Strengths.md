# Audit Part 1 — Compliance & Strengths

> **Audited files:** `User.h/cpp`, `Customer.h`, `Manager.h`, `Salesman.h`, `Surveyor.h`,
> `Vehicle.h`, `InsurancePolicy.h`, `Claim.h`, `InspectionReport.h`, `Workshop.h`,
> `FileHandler.h/cpp`, `UserController.h/cpp`, `ClaimController.h/cpp`,
> `UIManager.h/cpp`, `AutoInsuranceSystem.cpp`

---

## 1. Feature Compliance Checklist

| # | Prompt Requirement | Status | Evidence |
|---|---|---|---|
| 1 | Store data about **customers** | ✅ PASS | `Customer.h` entity; persisted in `users.txt` via `UserController::registerCustomer` |
| 2 | Store data about **vehicles** | ✅ PASS | `Vehicle.h` entity; persisted in `vehicles.txt` via `UserController::registerVehicle` |
| 3 | Store data about **insurance details (policies)** | ✅ PASS | `InsurancePolicy.h` entity; persisted in `policies.txt` via `UserController::issuePolicy` |
| 4 | Store data about **claims** | ✅ PASS | `Claim.h` entity; persisted in `claims.txt` via `ClaimController::fileClaim` |
| 5 | Store data about **inspections** | ✅ PASS | `InspectionReport.h` entity; persisted in `reports.txt` via `ClaimController::submitInspectionReport` |
| 6 | Store data about **staff — Salesmen** | ✅ PASS | `Salesman.h` inherits from `User`; drives the Salesman tab |
| 7 | Store data about **staff — Surveyors** | ✅ PASS | `Surveyor.h` inherits from `User`; drives the Surveyor tab |
| 8 | Store data about **registered workshops** | ✅ PASS | `Workshop.h` entity; `workshops.txt` data file |
| 9 | **Damaged vehicle repaired only at a registered workshop** | ✅ PASS | `submitInspectionReport` calls `validateWorkshopRegistration` and blocks on failure |
| 10 | **Manager views list of new customers** | ✅ PASS | `UserController::fetchNewCustomersReport` |
| 11 | **Manager views pending claims** | ✅ PASS | `ClaimController::fetchPendingClaims` |
| 12 | **Manager views inspection reports** | ⚠️ PARTIAL | `reports.txt` is written correctly; no dedicated "Fetch All Reports" manager method or UI button exists |
| 13 | **Manager approves a claim after examining inspection report** | ✅ PASS | `approveClaim` enforces report prerequisite; in-place status flip |
| 14 | **Manager views previous claims by a customer** | ✅ PASS | `ClaimController::fetchCustomerClaimHistory` (3-table join) |
| 15 | **File-based data persistence (no DBMS)** | ✅ PASS | Plain `.txt` files only |
| 16 | **No DBMS used** | ✅ PASS | Only `std::fstream` throughout |
| 17 | **Business layer (classes)** | ✅ PASS | `UserController` and `ClaimController` form the complete business tier |
| 18 | **UI does not directly load/store files** | ✅ PASS | `UIManager.cpp` contains zero file I/O — only controller calls |
| 19 | **Salesman: Register customer** | ✅ PASS | `UserController::registerCustomer` |
| 20 | **Salesman: Register vehicle** | ✅ PASS | `UserController::registerVehicle` |
| 21 | **Salesman: Issue policy** | ✅ PASS | `UserController::issuePolicy` |
| 22 | **Customer: File a claim** | ✅ PASS | `ClaimController::fileClaim` |
| 23 | **Customer: View own policies** | ✅ PASS | `UserController::fetchCustomerPolicies` |
| 24 | **Surveyor: Submit inspection report** | ✅ PASS | `ClaimController::submitInspectionReport` |
| 25 | **Language is C++** | ✅ PASS | All `.cpp`/`.h`; compiled with MSVC via CMake |
| 26 | **Simple files for persistence** | ✅ PASS | Comma-delimited `.txt` files |
| 27 | **UI need not be graphical** | ✅ BONUS | System exceeds requirement with a full ImGui dashboard |
| 28 | **Registered-workshop enforcement** | ✅ PASS | `validateWorkshopRegistration` enforces flag == "1" |
| 29 | **Claim status tracking** (Pending → Approved) | ✅ PASS | In-place update via `FileHandler::updateRecord` |
| 30 | **Claim `Rejected` status** | ❌ MISSING | Documented in `Claim.h` comment but no controller method or UI action ever sets Rejected |

**Score: 27 / 30 fully implemented. 1 partial (#12). 2 missing (#30 + see Part 2).**

---

## 2. Architectural Review — 3-Tier Separation

### Tier 1 — Presentation Layer (`UIManager.h/cpp`)

`UIManager.cpp` contains **zero** `#include <fstream>`, `std::ifstream`, `std::ofstream`, or any direct file I/O call. Every data operation is a controller method call:

```
UIManager → userCtrl.registerCustomer(...)           ✔ controller call only
UIManager → claimCtrl.approveClaim(...)              ✔ controller call only
UIManager → claimCtrl.fetchCustomerClaimHistory(...) ✔ controller call only
```

**Verdict: The wall between UI and file I/O is absolute and architecturally correct.**

### Tier 2 — Business Logic Layer (`UserController`, `ClaimController`)

Both controllers hold a **private** `FileHandler fileHandler` instance — encapsulated and never exposed upward to the UI. All domain rules live here:

- Workshop validation before report submission  
- Inspection-report prerequisite before claim approval  
- Multi-table join for customer claim history

### Tier 3 — Data Access Layer (`FileHandler`)

`FileHandler` is pure I/O — it has no knowledge of what a Claim or Customer is. It provides three operations:

- `saveRecord` — append a CSV line  
- `loadRecords` — return all lines as `vector<string>`  
- `updateRecord` — read-modify-write with truncate

### Dependency Direction (no violations detected)

```
AutoInsuranceSystem.cpp
        │
        ▼
  UIManager (Presentation)
    │           │
    ▼           ▼
UserController  ClaimController   (Business Logic)
    │                │
    └──────┬──────────┘
           ▼
       FileHandler                (Data Access)
           │
           ▼
     *.txt flat files
```

No upward or cross-tier dependencies exist. The architecture is clean.

---

## 3. Strong Points

### 3.1 Inheritance & Polymorphism — Textbook Quality
`User` is an abstract base class with a **pure virtual** `getRole()`. All four role types correctly `override` it. The **virtual destructor** prevents undefined behavior during polymorphic deletion — a detail many students miss.

### 3.2 Const-Correctness Throughout
All entity getters are `const`. `authenticate()` takes `const std::string&`. `generateID()` is `const`. This is correct, professional C++ that professors specifically look for.

### 3.3 In-Place File Update — No Stale Duplicate Records
`FileHandler::updateRecord` implements the correct read-modify-write pattern using `std::ios::trunc`. The previous append-only design would have created corrupt duplicate entries. The current implementation is reliable.

### 3.4 Auto-ID Generation Eliminates an Entire Bug Class
Removing manual ID input from the UI and generating `PREFIX-timestamp-rand` IDs in the controller eliminates: duplicate IDs, empty IDs, and IDs containing commas that would break CSV parsing.

### 3.5 Business Rules Enforced at Controller Tier
Two hard rules are enforced in the business layer and **cannot be bypassed by the UI**:
1. Workshop must be registered before an inspection report can be saved.
2. An inspection report must exist before a claim can be approved.

### 3.6 Zero-Crash File Loading
`FileHandler::loadRecords` returns an empty vector if the file does not exist — no exceptions, no `nullptr`, no abort. The system starts cleanly with no data files present.

### 3.7 `#pragma once` Used Consistently
All headers use `#pragma once`, which is cleaner than traditional `#ifndef` guards and is fully supported by MSVC, GCC, and Clang.

### 3.8 Correct `FileHandler` as a DAL Adapter
All `std::fstream` work is isolated to one class. Swapping the storage backend would require changes only to `FileHandler`, not to any controller or UI — this is textbook Separation of Concerns.
