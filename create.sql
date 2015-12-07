DROP DATABASE IF EXISTS project;
CREATE DATABASE project;

USE project;

CREATE AGGREGATE FUNCTION SUM_HE RETURNS STRING SONAME 'SUM_HE.so';

CREATE TABLE Employees (
	id int,
    age int,
    salary int(7) NOT NULL,
    PRIMARY KEY(id));
