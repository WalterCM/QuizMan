DROP TABLE Topics;

CREATE TABLE `Topics` (
	`TopicID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`TopicName`	TEXT NOT NULL,
	`SubjectID`	INTEGER NOT NULL,
	FOREIGN KEY(`SubjectID`) REFERENCES Subjects ( SubjectID )
);

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Expresiones Algebraicas');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Ecuaciones lineales');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Inecuaciones');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Sistemas de ecuaciones');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Polinomios');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Funciones');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Factorizacion');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Ecuaciones cuadraticas');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Numeros racionales');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Numeros irracionales');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Funciones polinomicas');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Ecuaciones con radicales');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Logaritmos');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Funciones exponenciales');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Funciones logaritmicas');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Numeros imaginarios y complejos');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (1, 'Matrices');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (2, 'Adicion y sustraccion');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (2, 'Multiplicacion y division');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (2, 'Numeros negativos y valor absoluio');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (2, 'Decimales');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (2, 'Fracciones');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Lineas');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Angulos');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'El plano cartesiano');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'El teorema de pitagoras');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Transformaciones, congruencia y similaridad');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Angulos e intersecciones');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Propiedades de los triangulos');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Cuadrilateros');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Transformaciones');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Congruencia');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Similaridad');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Circulos');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Perimetro');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Area');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Volumen');

INSERT INTO Topics (SubjectID, TopicName)
VALUES (3, 'Geometria analitica');