
CREATE DATABASE outlimit DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE outlimit;

create table outlimit(
       id               INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
       log_time         VARCHAR(64) NOT NULL,
       machine_name     VARCHAR(64) NOT NULL,
       domain_name      VARCHAR(64) NOT NULL,
       send_account     VARCHAR(64) NOT NULL,
       max_account_num  INT NOT NULL,
       now_account_num  INT NOT NULL,
       send_num         INT NOT NULL,
       evidence         text,
       CONSTRAINT one_recred UNIQUE(log_time,machine_name,send_account)
       -- 一台机器上的一个账号一天只能有一条记录
);