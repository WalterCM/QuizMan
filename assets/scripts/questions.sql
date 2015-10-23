DROP TABLE Questions;

CREATE TABLE `Questions` (
	`QuestionID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`QuestionDescription`	TEXT NOT NULL,
	`QuestionImageLocation`	TEXT
);

INSERT INTO Questions (QuestionDescription)
VALUES ('Cuanto es 2 + 2');

INSERT INTO Questions (QuestionDescription)
VALUES ('Que es un angulo rectangulo');