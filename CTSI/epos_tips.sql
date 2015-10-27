--修改超爽提示语
delete from epos_tips where menu_id in(435);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,435,'系统升级，空充业务暂停',0);
commit;


--开广州售卡
delete from epos_tips where menu_id in(116,403,404,556);
commit;
--关广州华鸿移动
delete from epos_tips where menu_id in(150,414,415,416,417,418);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,150,'移动运营商系统升级，空充业务暂停，请使用30 50 100的电子售卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,414,'移动运营商系统升级，空充业务暂停，请使用30 50 100的电子售卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,415,'移动运营商系统升级，空充业务暂停，请使用30 50 100的电子售卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,416,'移动运营商系统升级，空充业务暂停，请使用30 50 100的电子售卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,417,'移动运营商系统升级，空充业务暂停，请使用30 50 100的电子售卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,418,'移动运营商系统升级，空充业务暂停，请使用30 50 100的电子售卡',1);
commit;
--关广州华鸿电信
delete from epos_tips where menu_id in(574,575,576);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,574,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,575,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,576,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
commit;


--开东莞售卡
delete from epos_tips where menu_id in(455,501,502);
commit;
--关东莞华鸿移动
delete from epos_tips where menu_id in(461,504,505,506,507);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,461,'移动运营商系统升级，空充业务暂停，请使用50 100的电子卷',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,504,'移动运营商系统升级，空充业务暂停，请使用50 100的电子卷',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,505,'移动运营商系统升级，空充业务暂停，请使用50 100的电子卷',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,506,'移动运营商系统升级，空充业务暂停，请使用50 100的电子卷',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,507,'移动运营商系统升级，空充业务暂停，请使用50 100的电子卷',1);
commit;
--关东莞华鸿电信
delete from epos_tips where menu_id in(508,460,518,519,520,569,570,571,574,575,576);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,508,'移动运营商系统升级，空充业务暂停，请使用50 100的电子卷',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,460,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,518,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,519,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,520,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,569,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,570,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,571,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,574,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,575,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,576,'电信空充平台升级，电信空充暂停使用，请使用充值付费卡',1);
commit;


--开广州华鸿移动
delete from epos_tips where menu_id in(150,414,415,416,417,418);
commit;
--开广州华鸿电信
delete from epos_tips where menu_id in(574,575,576);
commit;
--关广州售卡
delete from epos_tips where menu_id in(116,403,404,556);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,116,'移动电子卷已售完  请使用移动直充！',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,403,'移动电子卷已售完  请使用移动直充！',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,404,'移动电子卷已售完  请使用移动直充！',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,556,'移动电子卷已售完  请使用移动直充！',1);
commit;


--开东莞华鸿移动
delete from epos_tips where menu_id in(461,504,505,506,507);
commit;
--开东莞华鸿电信
delete from epos_tips where menu_id in(508,460,518,519,520,569,570,571,574,575,576);
commit;
--关东莞售卡
delete from epos_tips where menu_id in(455,501,502);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,455,'移动电子卷已售完  请使用移动直充！',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,501,'移动电子卷已售完  请使用移动直充！',1);
insert into epos_tips(epos_or_ipos,menu_id,tips,return) values(0,502,'移动电子卷已售完  请使用移动直充！',1);
commit;
