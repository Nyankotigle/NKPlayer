#爬取豆瓣相册

import requests
import re
from bs4 import BeautifulSoup
import os               #引入os库处理文件路径
import traceback        #跟踪异常返回信息
from urllib import request

import subprocess as sp
from lxml import etree
import random
from time import sleep

"""
函数说明:检查代理IP的连通性
Parameters:
    ip - 代理的ip地址
    lose_time - 匹配丢包数
    waste_time - 匹配平均时间
Returns:
    average_time - 代理ip平均耗时
"""
def check_ip(ip, lose_time, waste_time):
    #命令 -n 要发送的回显请求数 -w 等待每次回复的超时时间(毫秒)
    cmd = "ping -n 3 -w 3 %s"
    #执行命令
    p = sp.Popen(cmd % ip, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE, shell=True)
    #获得返回结果并解码
    out = p.stdout.read().decode("gbk")
    #丢包数
    lose_time = lose_time.findall(out)
    #当匹配到丢失包信息失败,默认为三次请求全部丢包,丢包数lose赋值为3
    if len(lose_time) == 0:
        lose = 3
    else:
        lose = int(lose_time[0])
    #如果丢包数目大于2个,则认为连接超时,返回平均耗时1000ms
    if lose > 2:
        #返回False
        return 1000
    #如果丢包数目小于等于2个,获取平均耗时的时间
    else:
        #平均时间
        average = waste_time.findall(out)
        #当匹配耗时时间信息失败,默认三次请求严重超时,返回平均耗时1000ms
        if len(average) == 0:
            return 1000
        else:
            #
            average_time = int(average[0])
            #返回平均耗时
            return average_time

"""
函数说明:初始化正则表达式
Returns:
    lose_time - 匹配丢包数
    waste_time - 匹配平均时间
"""
def initpattern():
    #匹配丢包数
    lose_time = re.compile(u"丢失 = (\d+)", re.IGNORECASE)
    #匹配平均时间
    waste_time = re.compile(u"平均 = (\d+)ms", re.IGNORECASE)
    return lose_time, waste_time


def get_newproxy():
    #如果平均时间超过100ms重新选取ip
    while True:
        #从所有IP中随机选取一个IP作为代理进行访问
        proxy = random.choice(proxys_list)
        split_proxy = proxy.split('#')
        #获取IP
        ip = split_proxy[1]
        #检查ip
        average_time = check_ip(ip, lose_time, waste_time)
        #if average_time > 100:
            #去掉不能使用的IP
            #proxys_list.remove(proxy)
            #print("ip连接超时, 重新获取中!")
        if average_time < 100:
            break

    #去掉已经使用的IP
    #proxys_list.remove(proxy)
    proxy_dict = {split_proxy[0]:split_proxy[1] + ':' + split_proxy[2]}
    #print("使用代理:", proxy_dict)    
    return proxy_dict


def getAllPostURL(myheader,mycookie):
    URLList = []
    NameList = []

    URL_LIST  = ["62067009"]
    PAGE_LIST = [ 251      ]
    ROOT_LIST = ["人物"     ]
        
    for k in range(len(URL_LIST)):        
        print("###开始下载图集:  ", URL_LIST[k]," ",PAGE_LIST[k]," ",ROOT_LIST[k])
        
        root = "D://"+ ROOT_LIST[k] +"//"                          #保存海报的文件夹
        page_url0 = "https://www.douban.com/photos/album/" + URL_LIST[k] + "/?m_start="
        #每页18个
        for i in range(74,PAGE_LIST[k]):  #页数
            page_url = page_url0 + str(i*18) #构造每一页的URL
            print("开始下载第",i+1,"页")
            try:
                sleep(random.random()/2 + 0.5)
                proxies = get_newproxy() #获取新的ip代理
                response2 = requests.get(page_url,cookies=mycookie,headers=myheader,proxies=proxies,timeout=10)
                #print(response2.status_code) 
                response2.raise_for_status()
                page_html = response2.text
                soup2 = BeautifulSoup(page_html, "html.parser")
                #<div class="photo_wrap">
                #<img width="142" src="https://img9.doubanio.com/view/photo/m/public/p2249898444.webp" />
                div_list = soup2.findAll('div', attrs={'class':'photo_wrap'})        
                for div in div_list:
                    #每个div标签中的a标签有具体的图片链接
                    img_url = div.findAll('img')[0]['src'].replace("/m/","/l/")                
                    #img_name = div.findAll('a')[0]['title']                
                    #print(img_url)
                    path = root + re.split("/",img_url)[-1].replace("webp","jpg")
                    #path = root + img_name +".jpg"
                    try:
                        sleep(random.random()/2 + 0.5)
                        post = requests.get(img_url,cookies=mycookie,headers=myheader,proxies=proxies,timeout=10)                            
                        #print(post.status_code) 
                        post.raise_for_status()
                        with open(path, 'wb') as f:     #打开path指向的文件，用f标记
                                    f.write(post.content)       #以二进制形式将获取内容写入文件
                                    f.close()                   #关闭文件
                                    #print("文件保存成功")
                                
                    except:
                        print("海报文件获取失败")
                        #traceback.print_exc()           
                                       
            except:
                print("第",i,"页获取失败")
                #traceback.print_exc()


if __name__ == '__main__':
    #初始化正则表达式
    lose_time, waste_time = initpattern()
    '''
    #获取IP代理
    print("正在获取代理池...")
    proxys_list = get_proxys()
    print("代理池获取完毕")
    #print(proxys_list)
    '''
    #获取IP代理
    with open("D:\\Proxy1.txt", 'r') as f:
            Proxies = f.read()
            proxys_list = Proxies.split('\n')
    print("共获取",len(proxys_list),"条代理")
    #print(proxys_list)
    
    MYcookies = {
    'll': '118137',
    'bid': '6OZ-Gp0OAVk',
    '_vwo_uuid_v2': 'D3DC7C5B4FDA2F66823979E9040DD75B8|6c02d60ec66d62ecb62a890f80aee90a',
    'douban-fav-remind': '1',
    '__yadk_uid': 'DxbRllrIyam2C9iMClgeHjkbFiGMGcfD',
    '__gads': 'ID=2acba52515ad8f1d:T=1558627222:S=ALNI_MY7QAxJJfIq1c0UjL0q51e39rcT4w',
    'push_doumail_num': '0',
    'push_noty_num': '0',
    'ct': 'y',
    'douban-profile-remind': '1',
    '__utmv': '30149280.14544',
    '__utmc': '30149280',
    'dbcl2': '145449233:dwwRntsTyQU',
    'ck': 'kNuc',
    '__utma': '30149280.772985710.1581559743.1586477800.1586486521.35',
    '__utmz': '30149280.1586486521.35.10.utmcsr=accounts.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/passport/login',
    '_pk_ref.100001.8cb4': '%5B%22%22%2C%22%22%2C1586486568%2C%22https%3A%2F%2Faccounts.douban.com%2Fpassport%2Flogin%3Fredir%3Dhttps%253A%252F%252Fwww.douban.com%252Fpeople%252FgoodnightXX%252Fphotos%22%5D',
    '_pk_id.100001.8cb4': '731ed5ab87e5e0ee.1558105471.40.1586486568.1586477852.',
    '_pk_ses.100001.8cb4': '*',
    '__utmt': '1',
    '__utmb': '30149280.2.10.1586486521',
    }

    MYheaders = {
    'Connection': 'keep-alive',
    'Cache-Control': 'max-age=0',
    'Upgrade-Insecure-Requests': '1',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36',
    'Sec-Fetch-Dest': 'document',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9',
    'Sec-Fetch-Site': 'none',
    'Sec-Fetch-Mode': 'navigate',
    'Sec-Fetch-User': '?1',
    'Accept-Language': 'zh-CN,zh;q=0.9',
    }
    getAllPostURL(MYheaders,MYcookies)
    
