DROP TABLE Subjects;

CREATE TABLE `Subjects` (
	`SubjectID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`SubjectName`	TEXT NOT NULL,
	`AreaID` INTEGER NOT NULL,
	FOREIGN KEY(`AreaID`) REFERENCES Areas ( AreaID )
);

INSERT INTO Subjects (SubjectName, AreaID)
VALUES ('Algebra', 1);

INSERT INTO Subjects (SubjectName, AreaID)
VALUES('Aritmetica', 1);

INSERT INTO Subjects (SubjectName, AreaID)
VALUES('Geometria', 1);