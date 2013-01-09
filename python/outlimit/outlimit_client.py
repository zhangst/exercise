#!/usr/bin/python
# -*- coding:utf-8 -*-

import re
import os
import sys
import time
import socket

# 配置
SERVER_IP='58.68.254.38'
SERVER_PORT=1234


def get_account(domain):
    '''调用命令行获取 最大用户数和当前用户数'''
    
    try:
        sys_str = "/quark/bin/domainmgr --list " + domain
        pipe = os.popen(sys_str)
        content = pipe.read().decode('gbk')
        pipe.close()
    except Exception, e:
        return 0,0

    res = re.search(ur'[\s\S]+最大用户数:\s*(\d+)[\s\S]+当前用户数:\s*(\d+)[\s\S]+', content, re.I)
    if res:
        return int(res.group(1)),int(res.group(2))
    else:
        return 0,0


def deliver(log, mail, maxnum):
    '''统计本账号发信封数，并截取证据'''
    num = 0
    evidence = ""

    re_str = ".+REMOTE: S:" + mail + ".+ SUB:.+"
    num_re = re.compile(re_str)
    for line in open(log):
        res = num_re.search(line)
        if res:
            if num <= maxnum:
                try:
                    evidence += line.decode('gbk')
                    num += 1
                except Exception, e:
                    pass
    evidence = re.sub(r'[\/:*?"<>|]', '-', evidence)
    return num, evidence


if __name__ == '__main__':
    if 2 == len(sys.argv):
        log_time = sys.argv[1]
    elif 1 == len(sys.argv):
        # 默认抓取昨天日志
        log_time = time.strftime("%Y-%-m-%-d", time.localtime(time.time() - 24*60*60))
    else:
        sys.exit(1)

    # 判断文件是否存在
    smtpd_log = '/quark/log/quark-smtpd.log_' + log_time
    deliver_log = '/quark/log/quark-deliver.log_' + log_time
    if not os.path.exists(smtpd_log):
        print "%s do not exists." %(smtpd_log)
        sys.exit(1)
    if not os.path.exists(deliver_log):
        print "%s do not exists." %(deliver_log)
        sys.exit(1)

    print smtpd_log
    print deliver_log
    
    # 遍历log获取超限发件人地址
    mail_re = re.compile(ur'.+Out of recipients limit.+mail:\s?(.+?@.+?),.+')
    mail_dict = {}
    for line in open(smtpd_log):
        mail = mail_re.search(line, re.I)
        if mail:
            mail_dict[mail.group(1)] = 1

    print 'after outlimt user search'
    
    hostname = socket.gethostname()
    for mail in mail_dict:
        domain = mail.split('@')[1]
        max_account_num, now_account_num = get_account(domain)
        send_num, evidence = deliver(deliver_log, mail, 30)

        # 新记录直接插入，已有记录更新部分字段
        sql = 'insert into outlimit(log_time,machine_name,domain_name,send_account,max_account_num,now_account_num,send_num,evidence) values("%s", "%s", "%s", "%s", %d, %d, %d, "%s") on duplicate key update max_account_num=%d,now_account_num=%d,send_num=%d,evidence="%s";' %(log_time, hostname, domain, mail, max_account_num, now_account_num, send_num, evidence, max_account_num, now_account_num, send_num, evidence)
        sql = sql.encode('utf-8')

        server = socket.socket()
        server.connect((SERVER_IP, SERVER_PORT))
        server.send(sql)
        server.close()
