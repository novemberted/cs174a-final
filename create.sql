
CREATE AGGREGATE FUNCTION SUM_HE
RETURNS STRING SONAME "SUM_HE.so";

USE project;
CREATE TABLE Employees (
	id int,
    age int,
    salary int(7) NOT NULL,
    PRIMARY KEY(id));
