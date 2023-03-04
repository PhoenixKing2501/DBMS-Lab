-- Insert data into the tables to start with

-- Physician Table
INSERT INTO Physician 
VALUES
        ('DOC1','Alan Donald','Intern',111111111),
        ('DOC2','Bruce Reid','Attending Physician',222222222),
        ('DOC3','Courtney Walsh','Surgeon Physician',333333333),
        ('DOC4','Malcom Marshall','Senior Physician',444444444),
        ('DOC5','Dennis Lillee','Head Chief of Medicine',555555555),
        ('DOC6','Jeff Thomson','Surgeon Physician',666666666),
        ('DOC7','Richard Hadlee','Surgeon Physician',777777777),
        ('DOC8','Kapil Dev','Resident',888888888),
        ('DOC9','Ishant Sharma','Psychiatrist',999999999);

-- Department Table
INSERT INTO Department 
VALUES
        ('DEP1','Medicine','DOC4'),
        ('DEP2','Surgery','DOC7'),
        ('DEP3','Psychiatry','DOC9'),
        ('DEP4','Cardiology','DOC8');

-- Affiliated_With Table
INSERT INTO Affiliated_With 
VALUES
        ('DOC1','DEP1'),
        ('DOC2','DEP1'),
        ('DOC3','DEP1'),
        ('DOC3','DEP4'),
        ('DOC3','DEP2'),
        ('DOC4','DEP1'),
        ('DOC5','DEP1'),
        ('DOC6','DEP2'),
        ('DOC7','DEP1'),
        ('DOC7','DEP2'),
        ('DOC8','DEP1'),
        ('DOC9','DEP3');

-- MedProcedure Table
INSERT INTO "procedure" 
VALUES
        ('PRO1','Bypass Surgery',1500.0),
        ('PRO2','Angioplasty',3750.0),
        ('PRO3','Arthoscopy',4500.0),
        ('PRO4','Carotid Endarterectomy',10000.0),
        ('PRO5','Cholecystectomy',4899.0),
        ('PRO6','Tonsillectomy',5600.0),
        ('PRO7','Cataract Surgery',25000.0);


-- Patient Table
INSERT INTO patient 
VALUES
        (100000001,'Dilip Vengsarkar','42 Foobar Lane','555-0256','INS3','DOC1'),
        (100000002,'Richie Richardson','37 Infinite Loop','555-0512','INS1','DOC2'),
        (100000003,'Mark Waugh','101 Parkway Street','555-1204','INS1','DOC2'),
        (100000004,'Ramiz Raza','1100 Sparks Avenue','555-2048','INS9','DOC3');

-- Room Table
INSERT INTO Room
VALUES
        (101,'Single',1,1,FALSE),
        (102,'Single',1,1,FALSE),
        (103,'Single',1,1,FALSE),
        (111,'ICU',1,2,FALSE),
        (112,'ICU',1,2,TRUE),
        (113,'Single',1,2,FALSE),
        (121,'Single',1,3,FALSE),
        (122,'Single',1,3,FALSE),
        (123,'ICU',1,3,FALSE),
        (201,'Single',2,1,TRUE),
        (202,'Single',2,1,FALSE),
        (203,'Single',2,1,FALSE),
        (211,'Single',2,2,FALSE),
        (212,'Single',2,2,FALSE),
        (213,'ICU',2,2,TRUE),
        (221,'ICU',2,3,FALSE),
        (222,'Single',2,3,FALSE),
        (223,'Single',2,3,FALSE),
        (301,'ICU',3,1,FALSE),
        (302,'Single',3,1,TRUE),
        (303,'Single',3,1,FALSE),
        (311,'Single',3,2,FALSE),
        (312,'ICU',3,2,FALSE),
        (313,'Single',3,2,FALSE),
        (321,'Single',3,3,TRUE),
        (322,'Single',3,3,FALSE),
        (323,'ICU',3,3,FALSE),
        (401,'Single',4,1,FALSE),
        (402,'Single',4,1,TRUE),
        (403,'Single',4,1,FALSE),
        (411,'Single',4,2,FALSE),
        (412,'Single',4,2,FALSE),
        (413,'ICU',4,2,FALSE),
        (421,'Single',4,3,TRUE),
        (422,'Single',4,3,FALSE),
        (423,'ICU',4,3,FALSE);

-- Stay Table
INSERT INTO Stay VALUES(3215,100000001,111,'2018-05-01','2018-05-04');
INSERT INTO Stay VALUES(3216,100000003,123,'2018-05-03','2018-05-24');
INSERT INTO Stay VALUES(3217,100000004,112,'2018-05-02','2018-05-03');


-- Undergoes Table
INSERT INTO Undergoes VALUES(100000001,6,3215,'2018-05-02',3,101);
INSERT INTO Undergoes VALUES(100000001,2,3215,'2018-05-03',7,101);
INSERT INTO Undergoes VALUES(100000004,1,3217,'2018-05-07',3,102);
INSERT INTO Undergoes VALUES(100000004,5,3217,'2018-05-09',6,NULL);
INSERT INTO Undergoes VALUES(100000001,7,3217,'2018-05-10',7,101);
INSERT INTO Undergoes VALUES(100000004,4,3217,'2018-05-13',3,103);

-- Medication Table
INSERT INTO Medication VALUES(1,'Paracetamol','Z','N/A');
INSERT INTO Medication VALUES(2,'Actemra','Foolki Labs','N/A');
INSERT INTO Medication VALUES(3,'Molnupiravir','Bale Laboratories','N/A');
INSERT INTO Medication VALUES(4,'Paxlovid','Bar Industries','N/A');
INSERT INTO Medication VALUES(5,'Remdesivir','Donald Pharmaceuticals','N/A');

-- Appointment Table
INSERT INTO Appointment VALUES(13216584,100000001,101,3,'2018-04-24 10:00','2018-04-24 11:00','A');
INSERT INTO Appointment VALUES(26548913,100000002,101,2,'2018-04-24 10:00','2018-04-24 11:00','B');
INSERT INTO Appointment VALUES(36549879,100000001,102,1,'2018-04-25 10:00','2018-04-25 11:00','A');
INSERT INTO Appointment VALUES(46846589,100000004,103,4,'2018-04-25 10:00','2018-04-25 11:00','B');
INSERT INTO Appointment VALUES(59871321,100000004,NULL,4,'2018-04-26 10:00','2018-04-26 11:00','C');
INSERT INTO Appointment VALUES(69879231,100000003,103,2,'2018-04-26 11:00','2018-04-26 12:00','C');
INSERT INTO Appointment VALUES(76983231,100000001,NULL,3,'2018-04-26 12:00','2018-04-26 13:00','C');
INSERT INTO Appointment VALUES(86213939,100000004,102,9,'2018-04-27 10:00','2018-04-21 11:00','A');
INSERT INTO Appointment VALUES(93216548,100000002,101,2,'2018-04-27 10:00','2018-04-27 11:00','B');

-- Prescribes Table
INSERT INTO Prescribes VALUES(1,100000001,1,'2018-04-24 10:47',13216584,'5');
INSERT INTO Prescribes VALUES(2,100000003,5,'2018-04-24 10:47',13216584,'5');
INSERT INTO Prescribes VALUES(3,100000001,4,'2018-04-24 10:47',13216584,'5');
INSERT INTO Prescribes VALUES(4,100000002,5,'2018-04-24 10:47',13216584,'5');
INSERT INTO Prescribes VALUES(9,100000004,2,'2018-04-27 10:53',86213939,'10');
INSERT INTO Prescribes VALUES(9,100000004,2,'2018-04-30 16:53',NULL,'5');


-- User Table