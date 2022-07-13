#爬取豆列海报

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
函数说明:获取IP代理
Parameters:    page - 高匿代理页数,默认获取第一页
Returns:       proxys_list - 代理列表
"""
'''
#xici代理版
def get_proxys():
    #存储代理的列表
    proxys_list = []
    for page in range(20):
        #requests的Session可以自动保持cookie,不需要自己维护cookie内容
        S = requests.Session()
        #西祠代理高匿IP地址
        target_url = 'https://www.xicidaili.com/nn/%d' % page
        #完善的headers
        target_headers = {'Upgrade-Insecure-Requests':'1',
            'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36',
            'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Referer':'http://www.xicidaili.com/nn/',
            'Accept-Encoding':'gzip, deflate, sdch',
            'Accept-Language':'zh-CN,zh;q=0.8',
        }
        try:
            #get请求
            target_response = S.get(url = target_url, headers = target_headers)
            #utf-8编码
            target_response.encoding = 'utf-8'
            #获取网页信息
            target_html = target_response.text
            #获取id为ip_list的table
            bf1_ip_list = BeautifulSoup(target_html, 'lxml')
            bf2_ip_list = BeautifulSoup(str(bf1_ip_list.find_all(id = 'ip_list')), 'lxml')
            ip_list_info = bf2_ip_list.table.contents
            
            #爬取每个代理信息
            for index in range(len(ip_list_info)):
                if index % 2 == 1 and index != 1:
                    dom = etree.HTML(str(ip_list_info[index]))
                    ip = dom.xpath('//td[2]')
                    port = dom.xpath('//td[3]')
                    protocol = dom.xpath('//td[6]')
                    proxys_list.append(protocol[0].text.lower() + '#' + ip[0].text + '#' + port[0].text)
            sleep(2)
        except:
            print("第",page,"页IP代理获取失败")                            
            traceback.print_exc()  
    for proxys in proxys_list:
        with open("D://proxy.txt", "a+") as f:
            f.write(proxys+"\n")
            f.close()
    #返回代理列表
    return proxys_list
'''
#快代理版
def get_proxys():
    #存储代理的列表
    proxys_list = []
    for page in range(1,200):
        #requests的Session可以自动保持cookie,不需要自己维护cookie内容
        S = requests.Session()
        #西祠代理高匿IP地址
        target_url = 'https://www.kuaidaili.com/free/inha/%d/' % page
        #完善的headers
        target_headers = {'Upgrade-Insecure-Requests':'1',
            'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36',
            'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Referer':'https://www.kuaidaili.com/free/inha/4/',
            'Accept-Encoding':'gzip, deflate, sdch',
            'Accept-Language':'zh-CN,zh;q=0.8',
        }
        try:
            #get请求
            target_response = S.get(url = target_url, headers = target_headers)
            #utf-8编码
            target_response.encoding = 'utf-8'
            #获取网页信息
            target_html = target_response.text
            #获取id为ip_list的table
            soup1 = BeautifulSoup(target_html, 'lxml')
            ip_info_list = soup1.find('tbody').findAll('tr')
            
            #爬取每个代理信息
            for ip_info in ip_info_list:
                td_list = ip_info.findAll('td')
                ip = td_list[0].get_text()
                port = td_list[1].get_text()
                protocol = td_list[3].get_text()
                proxys_list.append(protocol.lower() + '#' + ip + '#' + port)
            sleep(2)
        except:
            print("第",page,"页IP代理获取失败")                            
            traceback.print_exc()  
    for proxys in proxys_list:
        with open("D://豆瓣海报//proxy04.08.txt", "a+") as f:
            f.write(proxys+"\n")
            f.close()
        
    #返回代理列表
    return proxys_list
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
        if average_time > 100:
            #去掉不能使用的IP
            proxys_list.remove(proxy)
            #print("ip连接超时, 重新获取中!")
        if average_time < 100:
            break

    #去掉已经使用的IP
    proxys_list.remove(proxy)
    proxy_dict = {split_proxy[0]:split_proxy[1] + ':' + split_proxy[2]}
    #print("使用代理:", proxy_dict)    
    return proxy_dict


def getAllPostURL(myheader,mycookie):
    URLList = []
    NameList = []
    root = "D://豆瓣电影海报高清//"                          #保存海报的文件夹
    
    #共29页每页25部电影
    #https://www.douban.com/doulist/3568953/?start=     *西部电影 6页
    #https://www.douban.com/doulist/1641439/?start=     *全球电影票房总榜
    #https://www.douban.com/doulist/44514350/?start=    *科幻电影 13页
    #https://www.douban.com/doulist/45227898/?start=    *历届奥斯卡 13页   
    #https://www.douban.com/doulist/1502465/?start=     *历届戛纳电影节金棕榈奖 4页
    #https://www.douban.com/doulist/3151123/?start=     *威尼斯金狮奖 4页
    #https://www.douban.com/doulist/1502467/?start=     *柏林金熊奖 4页
    #https://www.douban.com/doulist/44514352/?start=    *武侠电影 4页
    #https://www.douban.com/doulist/117597483/?start=   *好莱坞黄金时代 9页
    #https://www.douban.com/doulist/45080588/?start=    *优秀海报 4页
    #https://www.douban.com/doulist/1230074/?start=     *创意海报 2页
    #https://www.douban.com/doulist/210993/?start=      *被海报吸引 11页
    #https://www.douban.com/doulist/315171/?start=      *海报控 10页
    #https://www.douban.com/doulist/111628610/?start=   TSPDT最伟大的1000部电影2019 40页
    #https://www.douban.com/doulist/20992352/?start=    高分电视剧 8页
    #https://www.douban.com/doulist/44708616/?start=    8分以上武侠剧 2页
    #https://www.douban.com/doulist/41546224/?start=    高分纪录片 11页
    #https://www.douban.com/doulist/44940782/?start=    高分动漫 6页
    page_url0 = "https://www.douban.com/doulist/315171/?start="
    page_url2 = "&sort=seq&playable=0&sub_type="
    for i in range(10):
        page_url = page_url0 + str(i*25) + page_url2 #构造每一页的URL
        print("开始下载第",i+1,"页")
        try:
            sleep(random.random()/2 + 0.5)
            proxies = get_newproxy() #获取新的ip代理
            response2 = requests.get(page_url,cookies=mycookie,headers=myheader,proxies=proxies,timeout=10)
            #print(response2.status_code) 
            response2.raise_for_status()
            page_html = response2.text
            soup2 = BeautifulSoup(page_html, "html.parser")
            #每部电影页面链接放在 <div class="post">中的
            # <a href="https://movie.douban.com/subject/1303408/" target="_blank">标签中
            #找出所有的符合条件的div标签
            movies = soup2.findAll('div', attrs={'class':'post'})        
            for movie in movies:
                #每个div标签中的a标签有具体的电影页面链接
                movie_url = movie.findAll('a')[0]['href']
                #print(movie_url)
                try:
                    #获取具体电影页面
                    sleep(random.random()/2 + 0.5)
                    #proxies = get_newproxy() #获取新的ip代理
                    response3 = requests.get(movie_url,cookies=mycookie,headers=myheader,proxies=proxies,timeout=10)
                    #print(response3.status_code) 
                    response3.raise_for_status()
                    movie_html = response3.text
                    soup3 = BeautifulSoup(movie_html, "html.parser")
                    #获取电影海报页的链接
                    posts_url = soup3.find('a', attrs={'title':"点击看更多海报"})['href']
                    #print(posts_url)
                    #获取电影海报页面
                    sleep(random.random()/2 + 0.5)
                    response4 = requests.get(posts_url,cookies=mycookie,headers=myheader,proxies=proxies,timeout=10)
                    #print(response4.status_code) 
                    response4.raise_for_status()
                    posts_html = response4.text
                    soup4 = BeautifulSoup(posts_html, "html.parser")
                    #获取第一张海报的链接
                    post_url = soup4.find('ul', attrs={'class':"poster-col3 clearfix"}).find('img')['src']
                    #获取电影名称
                    #string.strip()删除字符串两边的空字符,作为文件名不能有:：<>/\|"*?                 
                    post_name = soup4.find('title').get_text().strip().replace("海报","")
                    post_name = re.sub(r'[(:)(：)(<)(>)(/)(\)(|)(")(*)(?)]','',post_name)
                    #print(post_name,post_url)
                    
                    path = root + post_name + ".jpg"  #构造文件保存路径
                    if not os.path.exists(root):
                        os.mkdir(root)            #当前根目录不存在则建立
                    if not os.path.exists(path):  #当前文件不存在则用get从url获取
                        #将海报链接中的/m/替换为/raw/即为原图链接
                        raw_url = post_url.replace("/m/","/raw/")
                        #print(raw_url)
                        try:
                            '''
                            cmd_word = "you-get " +"-o D:/豆瓣海报2/ "+"-O "+ post_name + ".jpg "+ raw_url 
                            os.system(cmd_word)
                            '''
                            sleep(random.random()/2 + 0.5)
                            post = requests.get(raw_url,cookies=mycookie,headers=myheader,proxies=proxies,timeout=10)                            
                            #print(post.status_code) 
                            post.raise_for_status()
                            with open(path, 'wb') as f:     #打开path指向的文件，用f标记
                                f.write(post.content)       #以二进制形式将获取内容写入文件
                                f.close()                   #关闭文件
                                #print("文件保存成功")
                            
                        except:
                            print("海报文件获取失败",movie_url)
                            saveERPageURL(movie_url, 1)
                            traceback.print_exc()           
                    else:
                        print("文件已存在")                    
                except:
                    print("海报链接获取失败",movie_url)
                    saveERPageURL(movie_url, 1)
                    traceback.print_exc()           
        except:
            print("第",i,"页获取失败",page_url)
            saveERPageURL(page_url, 0)
            traceback.print_exc()


#保存获取失败的链接
def saveERPageURL(erURL, erKind):    
    if erKind==0:               #整个页面获取失败
        file = "D://豆瓣海报//erPage.txt"    
    else:                       #单个电影项获取失败
        file = "D://豆瓣海报//erMovie.txt" 
    try:
        with open(file, "a+") as f:
            f.write(erURL+"\r\n")
            f.close()
    except Exception as e:
        print('保存记录失败：[{}]'.format(e))


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
    '__yadk_uid': 'E6bpFXKxcz7069d38Yi6ri00IvjiURJS',
    '_vwo_uuid_v2': 'D3DC7C5B4FDA2F66823979E9040DD75B8|6c02d60ec66d62ecb62a890f80aee90a',
    'trc_cookie_storage': 'taboola%2520global%253Auser-id%3D2210e30d-fc9f-4ba4-959b-d3f02fba123d-tuct3b48e2c',
    'douban-fav-remind': '1',
    '__gads': 'ID=2acba52515ad8f1d:T=1558627222:S=ALNI_MY7QAxJJfIq1c0UjL0q51e39rcT4w',
    'push_doumail_num': '0',
    'push_noty_num': '0',
    'ct': 'y',
    'douban-profile-remind': '1',
    '__utmv': '30149280.14544',
    'dbcl2': '145449233:6VEmuFFTvhU',
    'ck': 'R7hH',
    '_pk_ref.100001.4cf6': '%5B%22%22%2C%22%22%2C1586521089%2C%22https%3A%2F%2Faccounts.douban.com%2Fpassport%2Flogin%3Fredir%3Dhttps%253A%252F%252Fmovie.douban.com%252F%22%5D',
    '_pk_id.100001.4cf6': 'ddd44d0187ba6846.1581559742.32.1586521089.1586498631.',
    '_pk_ses.100001.4cf6': '*',
    '__utma': '30149280.772985710.1581559743.1586496503.1586521089.37',
    '__utmb': '30149280.0.10.1586521089',
    '__utmc': '30149280',
    '__utmz': '30149280.1586521089.37.12.utmcsr=accounts.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/passport/login',
    }

    MYheaders = {
    'Connection': 'keep-alive',
    'Cache-Control': 'max-age=0',
    'Upgrade-Insecure-Requests': '1',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36',
    'Sec-Fetch-Dest': 'document',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9',
    'Sec-Fetch-Site': 'same-site',
    'Sec-Fetch-Mode': 'navigate',
    'Sec-Fetch-User': '?1',
    'Referer': 'https://accounts.douban.com/passport/login?redir=https%3A%2F%2Fmovie.douban.com%2F',
    'Accept-Language': 'zh-CN,zh;q=0.9',
    }
    getAllPostURL(MYheaders,MYcookies)
    
