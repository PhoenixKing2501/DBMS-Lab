-- Create tables for the database

CREATE TABLE IF NOT EXISTS physician (
    EmployeeID      TEXT        PRIMARY KEY,
    Name            TEXT        NOT NULL,
    Position        TEXT        NOT NULL
);

CREATE TABLE IF NOT EXISTS "procedure" (
    Code            TEXT        PRIMARY KEY,
    Name            TEXT        NOT NULL, 
    Cost            INT         NOT NULL
);

CREATE TABLE IF NOT EXISTS medication (
    Code            TEXT        PRIMARY KEY,
    Name            TEXT        NOT NULL,
    Brand           TEXT        NOT NULL,
    Description     TEXT        NOT NULL
);

CREATE TABLE IF NOT EXISTS room (
    Number          INT         PRIMARY KEY,
    Type            TEXT        NOT NULL,
    Floor           INT         NOT NULL,
    Block           TEXT        NOT NULL,
    Available       BOOLEAN     NOT NULL
);

CREATE TABLE IF NOT EXISTS department (
    DepartmentID    TEXT        PRIMARY KEY,
    Name            TEXT        NOT NULL,
    Head            TEXT        NOT NULL,
    FOREIGN KEY (Head) REFERENCES physician (EmployeeID)
);

CREATE TABLE IF NOT EXISTS patient (
    SSN             INT         PRIMARY KEY,
    Name            TEXT        NOT NULL,
    Address         TEXT        NOT NULL,
    Phone           TEXT        NOT NULL,
    InsuranceID     TEXT        UNIQUE,
    PCP             TEXT        NOT NULL,
    FOREIGN KEY (PCP) REFERENCES physician (EmployeeID)
);

CREATE TABLE IF NOT EXISTS stay (
    StayID          TEXT        PRIMARY KEY,
    Patient         INT         NOT NULL,
    Room            INT         NOT NULL,
    "Start"         TIMESTAMP   NOT NULL,
    "End"           TIMESTAMP,
    FOREIGN KEY (Patient) REFERENCES patient (SSN),
    FOREIGN KEY (Room) REFERENCES room (Number)
);

CREATE TABLE IF NOT EXISTS affiliated_with (
    Physician       TEXT        NOT NULL,
    Department      TEXT        NOT NULL,
    PRIMARY KEY (Physician, Department),
    FOREIGN KEY (Physician) REFERENCES physician (EmployeeID),
    FOREIGN KEY (Department) REFERENCES department (DepartmentID)
);

CREATE TABLE IF NOT EXISTS appointment (
    AppointmentID   TEXT        PRIMARY KEY,
    Patient         INT         NOT NULL,
    Physician       TEXT        NOT NULL,
    "Start"         TIMESTAMP   NOT NULL,
    "End"           TIMESTAMP   NOT NULL,
    /* ExaminationRoom TEXT        NOT NULL, */
    FOREIGN KEY (Patient) REFERENCES patient (SSN),
    FOREIGN KEY (Physician) REFERENCES physician (EmployeeID)
);

CREATE TABLE IF NOT EXISTS prescribes (
    Physician       TEXT,
    Patient         INT,
    Medication      TEXT,
    "Date"          TIMESTAMP,
    Appointment     TEXT,
    Dose            TEXT        NOT NULL,
    PRIMARY KEY (Physician, Patient, Medication, "Date"),
    FOREIGN KEY (Physician) REFERENCES physician (EmployeeID),
    FOREIGN KEY (Patient) REFERENCES patient (SSN),
    FOREIGN KEY (Medication) REFERENCES medication (Code),
    FOREIGN KEY (Appointment) REFERENCES appointment (AppointmentID)
);

CREATE TABLE IF NOT EXISTS undergoes (
    Patient         INT,
    "Procedure"     TEXT,
    Stay            TEXT,
    "Date"     TIMESTAMP,
    Physician       TEXT         NOT NULL,
    filedata        BYTEA,
    PRIMARY KEY (Patient, "Procedure", Stay, "Date"),
    FOREIGN KEY (Patient) REFERENCES patient (SSN),
    FOREIGN KEY ("Procedure") REFERENCES "procedure" (Code),
    FOREIGN KEY (Stay) REFERENCES stay (StayID),
    FOREIGN KEY (Physician) REFERENCES physician (EmployeeID)
);


CREATE TABLE IF NOT EXISTS users (
    EmployeeID      TEXT        PRIMARY KEY,
    Name            TEXT        NOT NULL,
    EmployeeType    TEXT        NOT NULL,
    SSN             INT         UNIQUE NOT NULL,
    Address         TEXT        NOT NULL,
    Email           TEXT        NOT NULL,
    Password        TEXT        NOT NULL
);