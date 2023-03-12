-- Insert data into the tables to start with

-- Physician Table
INSERT INTO physician 
VALUES
        ('DOC1','Alan Donald','Intern'),
        ('DOC2','Bruce Reid','Attending Physician'),
        ('DOC3','Courtney Walsh','Surgeon Physician'),
        ('DOC4','Malcom Marshall','Senior Physician'),
        ('DOC5','Dennis Lillee','Head Chief of Medicine'),
        ('DOC6','Jeff Thomson','Surgeon Physician'),
        ('DOC7','Richard Hadlee','Surgeon Physician'),
        ('DOC8','Kapil Dev','Resident'),
        ('DOC9','Ishant Sharma','Psychiatrist');

-- Department Table
INSERT INTO department 
VALUES
        ('DEP1','Medicine','DOC4'),
        ('DEP2','Surgery','DOC7'),
        ('DEP3','Psychiatry','DOC9'),
        ('DEP4','Cardiology','DOC8');

-- Affiliated_With Table
INSERT INTO affiliated_With 
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
        (100000003,'Mark Waugh','101 Parkway Street','555-1204','INS5','DOC2'),
        (100000004,'Ramiz Raza','1100 Sparks Avenue','555-2048','INS9','DOC3');

-- Room Table
INSERT INTO room
VALUES
        (101,'Single',1,'A',TRUE),
        (102,'Single',1,'A',TRUE),
        (103,'Single',1,'A',TRUE),
        (111,'ICU',1,'B',TRUE),
        (112,'ICU',1,'B',TRUE),
        (113,'Single',1,'B',TRUE),
        (121,'Single',1,'C',TRUE),
        (122,'Single',1,'C',TRUE),
        (123,'ICU',1,'C',TRUE),
        (201,'Single',2,'A',TRUE),
        (202,'Single',2,'A',TRUE),
        (203,'Single',2,'A',TRUE),
        (211,'Single',2,'B',TRUE),
        (212,'Single',2,'B',TRUE),
        (213,'ICU',2,'B',TRUE),
        (221,'ICU',2,'C',TRUE),
        (222,'Single',2,'C',TRUE),
        (223,'Single',2,'C',TRUE),
        (301,'ICU',3,1,TRUE),
        (302,'Single',3,'A',TRUE),
        (303,'Single',3,'A',TRUE),
        (311,'Single',3,'B',TRUE),
        (312,'ICU',3,'B',TRUE),
        (313,'Single',3,'B',TRUE),
        (321,'Single',3,'C',TRUE),
        (322,'Single',3,'C',TRUE),
        (323,'ICU',3,'C',TRUE),
        (401,'Single',4,'A',TRUE),
        (402,'Single',4,'A',TRUE),
        (403,'Single',4,'A',TRUE),
        (411,'Single',4,'B',TRUE),
        (412,'Single',4,'B',TRUE),
        (413,'ICU',4,'B',TRUE),
        (421,'Single',4,'C',TRUE),
        (422,'Single',4,'C',TRUE),
        (423,'ICU',4,'C',TRUE);

-- Stay Table
INSERT INTO stay 
VALUES
        ('STY3215',100000001,111,'2018-05-01','2018-05-04'),
        ('STY3216',100000003,123,'2018-05-03','2018-05-24'),
        ('STY3217',100000004,112,'2018-05-02','2018-05-03');


-- Undergoes Table
INSERT INTO undergoes 
VALUES
        (100000001,'PRO6','STY3215','2018-05-02','DOC3', NULL),
        (100000001,'PRO2','STY3215','2018-05-03','DOC7', NULL),
        (100000004,'PRO1','STY3217','2018-05-07','DOC3', NULL),
        (100000004,'PRO5','STY3217','2018-05-09','DOC6', NULL),
        (100000001,'PRO7','STY3217','2018-05-10','DOC7', NULL),
        (100000004,'PRO4','STY3217','2018-05-13','DOC3', NULL);

-- Medication Table
INSERT INTO medication 
VALUES
        ('MED1','Paracetamol','Z','N/A'),
        ('MED2','Actemra','Foolki Labs','N/A'),
        ('MED3','Molnupiravir','Bale Laboratories','N/A'),
        ('MED4','Paxlovid','Bar Industries','N/A'),
        ('MED5','Remdesivir','Donald Pharmaceuticals','N/A');

-- Appointment Table
INSERT INTO appointment 
VALUES
        ('APT013',100000001,'DOC3','2023-03-14 10:00','2023-03-14 11:00'),
        ('APT026',100000002,'DOC2','2023-03-14 10:00','2023-03-14 11:00'),
        ('APT036',100000001,'DOC1','2023-03-15 10:00','2023-03-15 11:00'),
        ('APT046',100000004,'DOC4','2023-03-15 10:00','2023-03-15 11:00'),
        ('APT059',100000004,'DOC4','2023-03-16 10:00','2023-03-16 11:00'),
        ('APT069',100000003,'DOC2','2023-03-16 11:00','2023-03-16 12:00'),
        ('APT076',100000001,'DOC3','2023-03-16 12:00','2023-03-16 13:00'),
        ('APT086',100000004,'DOC9','2023-03-17 10:00','2023-03-11 11:00'),
        ('APT093',100000002,'DOC2','2023-03-17 10:00','2023-03-17 11:00');

-- Prescribes Table
INSERT INTO prescribes 
VALUES
        ('DOC1',100000001,'MED1','2018-04-24 10:47','APT013','5'),
        ('DOC2',100000003,'MED5','2018-04-24 10:47','APT013','5'),
        ('DOC3',100000001,'MED4','2018-04-24 10:47','APT013','5'),
        ('DOC4',100000002,'MED5','2018-04-24 10:47','APT013','5'),
        ('DOC9',100000004,'MED2','2018-04-27 10:53','APT086','10'),
        ('DOC9',100000004,'MED2','2018-04-30 16:53',NULL,'5');


-- User Table
INSERT INTO users
VALUES
        ('DOC1','Alan Donald','doctor',111111111, 'ABC Street, WB, India, 722102', 'swarupksms@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC2','Bruce Reid','doctor',222222222, 'ABC Street, WB, India, 722103', 'anamitra137@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC3','Courtney Walsh','doctor',333333333, 'XYZ Street, WB, India, 722104', 'utsavbasu2501@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC4','Malcom Marshall','doctor',444444444, 'XYZ Street, WB, India, 722104', 'temporarymail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC5','Dennis Lillee','doctor',555555555, 'ABC Street, WB, India, 722102', 'temporarymail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC6','Jeff Thomson','doctor',666666666,'ABC Street, WB, India, 722102', 'temporarymail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC7','Richard Hadlee','doctor',777777777, 'ABC Street, WB, India, 722102', 'temporarymail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC8','Kapil Dev','doctor',888888888, 'ABC Street, WB, India, 722102', 'temporarymail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DOC9','Ishant Sharma','doctor',999999999, 'ABC Street, WB, India, 722102', 'temporarymail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('ADM1','Pradosh Mitter','admin',800000001, 'ABC Street, WB, India, 722102', 'def@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('ADM2','Sherlock Holmes','admin',800000002, 'ABC Street, WB, India, 722102', 'mno@gmail.com','b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6''' ),
        ('DEO1','Byomkesh Bakshi','data entry operator',500000001, 'ABC Street, WB, India, 722102', 'pqr@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('DEO2','Raja Roychoudhury','data entry operator',500000002, 'ABC Street, WB, India, 722102', 'rst@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('FDO1','Trilokeshwar Shanku','front desk operator',600000001, 'ABC Street, WB, India, 722102', 'uvw@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6'''),
        ('FDO2','Lalmohan Ganguly','front desk operator',600000002, 'ABC Street, WB, India, 722102', 'cde@gmail.com', 'b''$2b$12$V2p5g77lpRW1gWvb9V96ou5rj2IQa2f1kxYNHobcUa80BA8F6/5T6''');