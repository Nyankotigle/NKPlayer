import requests
import re
from bs4 import BeautifulSoup
import os       #引入os库处理文件路径
import traceback                #跟踪异常返回信息

#header = {'user-agent':'Mozilla/5.0'}   #更改浏览器身份标识

MYcookies = {
    'll': '118137',
    'bid': '6OZ-Gp0OAVk',
    '__yadk_uid': 'E6bpFXKxcz7069d38Yi6ri00IvjiURJS',
    '_vwo_uuid_v2': 'D3DC7C5B4FDA2F66823979E9040DD75B8|6c02d60ec66d62ecb62a890f80aee90a',
    'trc_cookie_storage': 'taboola%2520global%253Auser-id%3D2210e30d-fc9f-4ba4-959b-d3f02fba123d-tuct3b48e2c',
    'douban-fav-remind': '1',
    '__gads': 'ID=2acba52515ad8f1d:T=1558627222:S=ALNI_MY7QAxJJfIq1c0UjL0q51e39rcT4w',
    'dbcl2': '145449233:l8dkVIhOO3Y',
    'push_noty_num': '0',
    'push_doumail_num': '0',
    '__utmv': '30149280.14544',
    'ck': 'DoK1',
    '__utmc': '30149280',
    'ct': 'y',
    'douban-profile-remind': '1',
    '__utmz': '30149280.1586063382.14.5.utmcsr=movie.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/people/145449233/collect',
    '_pk_ref.100001.4cf6': '%5B%22%22%2C%22%22%2C1586070668%2C%22https%3A%2F%2Fwww.baidu.com%2Flink%3Furl%3Dm8pCfanIC19VNS2ZO_rWqaPV3ZBlqkOAZdbq1HkGBm4ppAiWCHchA1m3teYvJUiz1bLXstc-haTc3j9mNvD2pq%26wd%3D%26eqid%3D82af033c00014506000000065e7c4c2f%22%5D',
    '_pk_ses.100001.4cf6': '*',
    '__utma': '30149280.772985710.1581559743.1586063382.1586070669.15',
    '__utmb': '30149280.0.10.1586070669',
    '_pk_id.100001.4cf6': 'ddd44d0187ba6846.1581559742.13.1586074145.1586066426.',
}

MYheaders = {
    'Connection': 'keep-alive',
    'Upgrade-Insecure-Requests': '1',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36',
    'Sec-Fetch-Dest': 'document',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9',
    'Sec-Fetch-Site': 'same-origin',
    'Sec-Fetch-Mode': 'navigate',
    'Sec-Fetch-User': '?1',
    'Referer': 'https://movie.douban.com/subject/10484041/',
    'Accept-Language': 'zh-CN,zh;q=0.9',
}


def getAllPostURL():
    URLList = []
    NameList = []
    root = "D://豆瓣海报//"               #定义根目录
    start_url = "https://movie.douban.com/people/145449233/collect"    
    global MYcookies
    global MYheaders
    while(1):
        try:
            response1 = requests.get(start_url,cookies=MYcookies,headers=MYheaders,timeout = 10)
            #print(response1.status_code) 
            response1.raise_for_status()
            start_html = response1.text
            soup1 = BeautifulSoup(start_html, "html.parser")    
            text = re.split('\(|\)', soup1.find('title').get_text())
            total_num = int(text[1])
            print("一共",total_num,"部电影")
            break
        except:
            print("初始页面连接失败")
            traceback.print_exc()           #查找失败，打印异常信息
    
    #每页15个
    page_url0 = "https://movie.douban.com/people/145449233/collect?start="
    page_url2 = "&sort=time&rating=all&filter=all&mode=grid"
    for i in range(int(total_num/15)):#int(total_num/15)
        page_url = page_url0 + str(i*15) + page_url2 #构造每一页的URL
        #print(page_url)
        try:
            response2 = requests.get(page_url , cookies=MYcookies,headers=MYheaders)
            #print(response2.status_code) 
            response2.raise_for_status()
            page_html = response2.text
            soup2 = BeautifulSoup(page_html, "html.parser")
            movies = soup2.findAll('a', attrs={'class':'nbg'})        
            for movie in movies:
                #每个具体的电影页面链接
                movie_url = movie['href']
                #print(movie_url)
                try:
                    response3 = requests.get(movie_url , cookies=MYcookies,headers=MYheaders)
                    #print(response3.status_code) 
                    response3.raise_for_status()
                    movie_html = response3.text
                    soup3 = BeautifulSoup(movie_html, "html.parser")
                    posts_url = soup3.find('a', attrs={'title':"点击看更多海报"})['href']
                    #print(posts_url)
                    response4 = requests.get(posts_url , cookies=MYcookies,headers=MYheaders)
                    #print(response4.status_code) 
                    response4.raise_for_status()
                    posts_html = response4.text
                    soup4 = BeautifulSoup(posts_html, "html.parser")
                    post_url = soup4.find('ul', attrs={'class':"poster-col3 clearfix"}).find('img')['src']
                    #string.strip()删除字符串两边的空字符
                    post_name = soup4.find('title').get_text().strip().replace("海报","")
                    #print(post_name,post_url)
                    #URLList.append(post_url)
                    #NameList.append(post_name)
                    
                    path = root + post_name + ".jpg"  #文件路径
                    if not os.path.exists(root):
                        os.mkdir(root)            #当前根目录不存在则建立
                    if not os.path.exists(path):  #当前文件不存在则用get从url获取            
                        raw_url = post_url.replace("/m/","/raw/")
                        #print(raw_url)
                        try:
                            post = requests.get(raw_url, cookies=MYcookies,headers=MYheaders)
                            #print(post.status_code) 
                            post.raise_for_status()
                            with open(path, 'wb') as f:     #打开path指向的文件，用f标记
                                f.write(post.content)          #r.content表示get获取内容的二进制形式，即以二进制形式将获取内容写入文件
                                f.close()                   #关闭文件
                                #print("文件保存成功")
                        except:
                            print("海报文件获取失败",movie_url)
                            saveERPageURL(movie_url, 1)
                            #traceback.print_exc()           
                    else:
                        print("文件已存在")                    
                except:
                    print("海报链接获取失败",movie_url)
                    saveERPageURL(movie_url, 1)
                    #traceback.print_exc()           
        except:
            print("第",i,"页获取失败",page_url)
            saveERPageURL(page_url, 0)
            #traceback.print_exc()
    #return URLList,NameList


#保存获取失败的链接
def saveERPageURL(erURL, erKind):    
    if erKind==0:               #整个页面获取失败
        file = "D://豆瓣海报//erPage.txt"    
    else:                       #单个电影项获取失败
        file = "D://豆瓣海报//erMovie.txt" 
    try:
        with open(file, "a+") as f:
            f.write(erURL+"\r\n")
    except Exception as e:
        print('保存记录失败：[{}]'.format(e))


'''
def downloadPost(URLList , NameList):
    global MYcookies
    global MYheaders
    root = "D://豆瓣海报//"               #定义根目录
    for i in range(len(URLList)-1):    
        path = root + NameList[i] + ".jpg"  #文件路径
        if not os.path.exists(root):
            os.mkdir(root)            #当前根目录不存在则建立
        if not os.path.exists(path):  #当前文件不存在则用get从url获取            
            raw_url = URLList[i].replace("/m/","/raw/")
            #print(raw_url)
            post = requests.get(raw_url, cookies=MYcookies,headers=MYheaders)
            #print(post.status_code) 
            post.raise_for_status()
            with open(path, 'wb') as f:     #打开path指向的文件，用f标记
                f.write(post.content)          #r.content表示get获取内容的二进制形式，即以二进制形式将获取内容写入文件
                f.close()                   #关闭文件
                print("文件保存成功")
        else:
            print("文件已存在")
'''
def main():
    '''
    postURLList = []
    postNameList = []
    postURLList , postNameList = getAllPostURL()
    downloadPost(postURLList, postNameList)
    '''
    getAllPostURL()
main()
    
