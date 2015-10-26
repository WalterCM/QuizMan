DROP TABLE TopicsQuestions;

CREATE TABLE `TopicsQuestions` (
	`TopicID`	INTEGER NOT NULL UNIQUE,
	`QuestionID`	INTEGER NOT NULL UNIQUE,
	FOREIGN KEY(`TopicID`) REFERENCES Topic ( TopicID ),
	FOREIGN KEY(`QuestionID`) REFERENCES Question ( QuestionID )
);

INSERT INTO TopicsQuestions
VALUES (18, 1);

INSERT INTO TopicsQuestions
VALUES (24, 2);