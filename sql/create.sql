#创建数据库
create database travelreserve;
use travelreserve;

#创建航班表
create table FLIGHTS(
	flightNum	varchar(10) not null,
	price		int check(price>0),
	numSeats	int check(numSeats>0),
	numAvail	int,
	FromCity	varchar(30),
	ArivCity	varchar(30),
	primary key(flightNum));
	
# 创建宾馆表
create table HOTELS(
	hotelNum	varchar(10) not null,
	location	varchar(50),
	price		int check(price>0),
	numRooms	int check(numRooms>0),
	numAvail	int,
	primary key(hotelNum));
	
	#创建班车表
	create table BUS(
	busNum		varchar(10) not null,
	location	varchar(50),
	price		int check(price>0),
	numSeats	int check(numSeats>0),
	numAvail	int,
	primary key(busNum));
	
	#创建用户表
	create table CUSTOMERS(
	custID		varchar(10) not null,
	custName	varchar(20) not null,
	primary key(custID));
	
	#创建订单信息表
	create table RESERVATIONS(
	resvNum		varchar(10) not null,
	custID		varchar(10) not null,
	resvType	int check(resvType in (1, 2, 3)),
	resvKey		varchar(10), 
	primary key(resvNum),
	foreign key(custID) references customers(custID));