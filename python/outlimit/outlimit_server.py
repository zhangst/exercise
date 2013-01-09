#!/usr/bin/python
# -*- coding:utf-8 -*-

import os
import sys
import logging
import MySQLdb
#from daemon import Daemon
from SocketServer import TCPServer, ForkingMixIn, StreamRequestHandler


class Server(ForkingMixIn, TCPServer):pass

class Handler(StreamRequestHandler):
    # mysql conn
    conn = None
    
    def handle(self):
        try:
            sql = self.rfile.read(20480)
            cur = conn.cursor()
            cur.execute(sql)
            cur.close()
            conn.commit()
        except Exception, e:
          addr = self.request.getpeername()
          log_str = "addr:" + addr + e
          logging.debug(log_str)

if __name__ == '__main__':
    logging.basicConfig(filename = os.path.join(os.getcwd(), 'outlimit_log.txt'), level = logging.DEBUG, filemode = 'w', format = '%(asctime)s - %(levelname)s: %(message)s')
    
    try:
        conn = MySQLdb.connect(host='localhost', user='root', passwd='q1w2e3r4', db='outlimit', port=3306, charset='utf8')
        Handler.conn = conn
        
        server = Server(('', 1234), Handler)
        server.serve_forever()
        
        conn.close()
    except Exception, e:
        log_str = 'execption error: ' + e
        logging.debug(log_str)
        sys.exit(0)
