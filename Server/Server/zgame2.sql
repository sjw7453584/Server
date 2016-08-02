-- --------------------------------------------------------
-- 主机:                           192.168.1.72
-- 服务器版本:                        5.7.11 - MySQL Community Server (GPL)
-- 服务器操作系统:                      Linux
-- HeidiSQL 版本:                  9.3.0.4984
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;



-- 导出  表 zgame2.Accout 结构
CREATE TABLE IF NOT EXISTS `Accout` (
  `accoutName` varchar(80) NOT NULL,
  `accoutPasswd` varchar(40) NOT NULL,
  `accoutId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`accoutName`),
  UNIQUE KEY `accoutIdKey` (`accoutId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 数据导出被取消选择。


-- 导出  表 zgame2.User 结构
CREATE TABLE IF NOT EXISTS `User` (
  `accoutId` int(10) unsigned NOT NULL DEFAULT '0',
  `roleId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `roleName` varchar(24) NOT NULL,
  `creatureType` int(10) unsigned NOT NULL DEFAULT '0',
  `sceneId` int(10) unsigned NOT NULL DEFAULT '0',
  `X` FLOAT NOT NULL DEFAULT '0',
  `Y` FLOAT NOT NULL DEFAULT '0',
  `createTimeStamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `lastTiliUpdateTimeStamp` timestamp DEFAULT "1980-01-01 00:00:00",
  `curFormId` int(10) unsigned NOT NULL DEFAULT '1',
  `gold` BIGINT(10) unsigned NOT NULL DEFAULT '0',
  `silver` BIGINT(10) unsigned NOT NULL DEFAULT '0',
  `copper` BIGINT(10) unsigned NOT NULL DEFAULT '0',
  `countryId` int(10) unsigned NOT NULL DEFAULT '0',
  `armyId` int(10) unsigned NOT NULL DEFAULT '0',
  `armyMemberType` int(10) unsigned NOT NULL DEFAULT '0',
  `armyOfficerType` int(10) unsigned NOT NULL DEFAULT '0',
  `offices` VARCHAR(500)   NULL DEFAULT NULL,
  `cityId` int(10) unsigned NOT NULL DEFAULT '0',
  `pkMode` TINYINT(4) NOT NULL DEFAULT '1',
  `pkpunishment` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  PRIMARY KEY (`roleId`),
  UNIQUE KEY `accoutKey` (`accoutId`,`roleId`),
  INDEX `country` (`countryId`),
  INDEX `army` (`armyId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


CREATE TABLE IF NOT EXISTS `Heroes` (
	`heroId` INT(10) UNSIGNED NOT NULL COMMENT '武将对应配置表id',
	`roleId` INT(10) UNSIGNED NOT NULL COMMENT '武将所属主公id',
	`uniqueId` BIGINT(20) UNSIGNED NOT NULL COMMENT '武将唯一id',
	`level` INT(10) UNSIGNED NOT NULL DEFAULT '1' COMMENT '武将或主公等级',
	`exp` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '武将或主公经验',
	`species` INT(10) UNSIGNED NOT NULL COMMENT '武将或主公职业',
	`country` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '武将所属阵营',
	`type` TINYINT(5) UNSIGNED NOT NULL COMMENT '0主公，1武将',
	`in_form` TINYINT(5) UNSIGNED NOT NULL DEFAULT '0' COMMENT '0不在阵，1在阵',
	`break_level` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '武将突破等级',
	`quality` INT(10) UNSIGNED NOT NULL COMMENT '主公或武将品质',
	`star_level` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '将星等级',
	`destiny_level` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '天命等级',
	`name` VARCHAR(24) NOT NULL COMMENT '名称',
	`hero_star_level` TINYINT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '武将星级段数 10段1个星',
	`create_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
	UNIQUE INDEX `rolekey` (`roleId`, `heroId`, `type`)
)
COMMENT='保存武将主公信息'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

CREATE TABLE IF NOT EXISTS `Task` (
	`roleId` INT(10) UNSIGNED NOT NULL COMMENT '用户角色id',
	`taskId` INT(10) UNSIGNED NOT NULL COMMENT '任务id',
	`progress` VARCHAR(500)  NOT NULL  COMMENT '任务进度',
	`state` INT(10) UNSIGNED NOT NULL COMMENT '任务状态',
	`accept_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '任务接取时间',
	`done_time` TIMESTAMP DEFAULT "1980-01-01 00:00:00" COMMENT '任务完成时间',
	UNIQUE INDEX `taskkey` (`roleId`, `taskId`)
)
COMMENT='保存用户任务信息'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

CREATE TABLE IF NOT EXISTS `Form` (
	`pos_uniqueId` VARCHAR(500) NOT NULL,
	`roleId` INT(10) UNSIGNED NOT NULL,
	`formId` INT(10) UNSIGNED NOT NULL,
	UNIQUE INDEX `unique_index` (`roleId`, `formId`)
)
COMMENT='阵形'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

CREATE TABLE IF NOT EXISTS `Bag` (
	`roleId` INT(10) UNSIGNED NOT NULL COMMENT '角色id',
	`unique_id` BIGINT(20) UNSIGNED NOT NULL COMMENT '道具唯一id',
	`id` INT(10) UNSIGNED NOT NULL COMMENT '物品id',
	`cnt` INT(10) UNSIGNED NOT NULL COMMENT '物品数量',
	`weared` INT(10) UNSIGNED NOT NULL COMMENT '是否已穿戴',
	UNIQUE INDEX `unique` (`roleId`, `unique_id`)
)
COMMENT='背包'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

CREATE TABLE IF NOT EXISTS `Interior` (
	`roleId` INT(10) UNSIGNED NOT NULL COMMENT '角色id',
	`level` INT(10) UNSIGNED NOT NULL DEFAULT '1' COMMENT '内政厅等级',
	`taskId` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '当前派遗任务id,0没有任务派遣',
	`starLevel` INT(10) UNSIGNED NOT NULL DEFAULT '1' COMMENT '当前派遗任务星级',
	`startTime` TIMESTAMP NOT NULL DEFAULT "1980-01-01 00:00:00"  COMMENT '当前派遗任务开始时间',
	`timeSpan` INT(10) UNSIGNED NOT NULL  COMMENT '当前派遗任务完成需要时间',
	`lastRewardTime` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP  COMMENT '上次取大厅产出铜币时间',
	`heroIds` VARCHAR(500) NOT NULL  COMMENT '执行当前派遗任务的英雄id',
	`leftTimes` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '剩余内政任务次数',
	`isStart` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '是否开始',
	UNIQUE INDEX `key` (`roleId`)
)
COMMENT='内政信息'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

CREATE TABLE IF NOT EXISTS `country` (
	`id` INT(10) UNSIGNED NOT NULL COMMENT '国家id',
	`money` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '国库资金',
	`power` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '国力',
	`has_call_war` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '宣战',
	`has_been_call_war` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '被宣战',
	`peer_country` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '国战对手',
	`popularity` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '民心',
	`friends` VARCHAR(50) NOT NULL DEFAULT '0' COMMENT '盟国id',
	`enemies` VARCHAR(50) NOT NULL DEFAULT '0' COMMENT '交恶国id',
	UNIQUE INDEX `unique` (`id`)
)
COMMENT='国家'
ENGINE=InnoDB
;

CREATE TABLE IF NOT EXISTS `Army` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '军团id',
	`level` INT(10) UNSIGNED NOT NULL DEFAULT '1' COMMENT '军团等级',
	`city` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '领地',
	`country` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '所属国家',
	`money` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '军团资金',
	`autoNewbie` INT(10) UNSIGNED NOT NULL DEFAULT '1',
	UNIQUE INDEX `unique` (`id`),
	INDEX `index` (`country`)
)
COMMENT='军团'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;
-- 数据导出被取消选择。
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
