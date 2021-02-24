use scv
go

if exists (
    select *
    from sysobjects
    where 
    name = 'scvACCOUNT'
    and 
    type = 'U'
)
execute("drop table scvACCOUNT")
go

create table scvACCOUNT (
  account_id   numeric(7, 0) identity,
  account_name varchar(30) not null,
  gender       char(1) not null,
  title        char(1) not null,
  first_name   varchar(30) not null,
  last_name    varchar(30) not null,
  email        varchar(50) not null
)
go

create unique clustered index scvACCOUNT_IDX1 on scvACCOUNT (account_id)
go

create unique index scvACCOUNT_IDX2 on scvACCOUNT (account_name)
go

insert into scvACCOUNT
(account_name, gender, title, first_name, last_name, email)
values
("c1", "m", "m", "claudey", "claudenstein", "claude.claudenstein@gmail.com")
go

insert into scvACCOUNT
(account_name, gender, title, first_name, last_name, email)
values
("winny", "m", "p", "winny", "adrian", "wah@gmail.com")
go

insert into scvACCOUNT
(account_name, gender, title, first_name, last_name, email)
values
("ian", "m", "d", "ian", "mian", "ian.mian@gmail.com")
go

grant select on scvACCOUNT to scvuser
go



