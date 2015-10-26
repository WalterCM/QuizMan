DROP TABLE Options;

CREATE TABLE `Options` (
	`OptionID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`OptionDescription`	TEXT NOT NULL,
	`QuestionID`	INTEGER NOT NULL,
	FOREIGN KEY(`QuestionID`) REFERENCES Questions ( QuestionID )
);

INSERT INTO Options (OptionDescription, QuestionID)
VALUES ('4', 1);

INSERT INTO Options (OptionDescription, QuestionID)
VALUES ('3', 1);

INSERT INTO Options (OptionDescription, QuestionID)
VALUES ('2', 1);

INSERT INTO Options (OptionDescription, QuestionID)
VALUES ('6', 1);

INSERT INTO Options (OptionDescription, QuestionID)
VALUES ('1', 1);