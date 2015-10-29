DROP TABLE Accounts;

CREATE TABLE `Accounts` (
	`AccountID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`AccountName`	TEXT NOT NULL
);

INSERT INTO Accounts (AccountName)
VALUES ('Guest');

INSERT INTO Accounts (AccountName)
VALUES ('Walter');