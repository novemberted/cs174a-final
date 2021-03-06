DROP DATABASE IF EXISTS project;
CREATE DATABASE project;

USE project;

DROP FUNCTION SUM_HE;
CREATE AGGREGATE FUNCTION SUM_HE RETURNS STRING SONAME "SUM_HE.so";

CREATE TABLE Employees (
	id int,
    age int,
    salary text NOT NULL,
    PRIMARY KEY(id));
