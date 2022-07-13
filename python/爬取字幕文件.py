from selenium import webdriver
import os
import re
from bs4 import BeautifulSoup
import requests
from time import sleep
import traceback                #跟踪异常返回信息


start_url = "http://www.zimuku.la/search?q="
Moviefolder = 'G:\\###电影'
movieFileList = []
movieDirList = []
sub_URL_List = []

for s in os.listdir(Moviefolder):                   #列出文件夹下的所有dir
    movieFileList.append(s)
    #print(s)
'''
    if os.path.isfile(os.path.join(Moviefolder,s)): #如果是文件
        movieFileList.append(s)                     #加入到文件列表
    if os.path.isdir(os.path.join(Moviefolder,s)):  #如果是文件夹
        movieDirList.append(s)                      #加入到文件夹列表

for home,dirs,files in os.walk(Moviefolder):
    for filename in files:
        movieFileList.append(filename)  #文件夹下的所有文件名(包括子文件夹中的文件）
        #print(filename)
        #movieFileList.append(os.path.join(home,filename)) #文件夹下的所有文件完整路径名(包括子文件夹中的文件）

    for dirname in dirs:
        movieDirList.append(dirname)    #文件夹下的所有文件夹名(包括子文件夹中的文件夹）

print(len(movieFileList))
for movie in movieFileList:
    print(movie)
print(len(movieDirList))
for moviedir in movieDirList:
    print(moviedir)
'''
print("共",len(movieFileList),"个电影文件")
print("搜索字幕中...")
header = {'user-agent':'Mozilla/5.0'}   #更改浏览器身份标识
Download_url_List = []
for movie in movieFileList:
    #[\u4e00-\u9fa5]表示任意中文字符
    #即将文件名中的第一串连续的任意个中文、英文或数字字符作为搜索关键词
    movie_name = re.findall(r'[\u4e00-\u9fa5A-Za-z0-9]+', movie)[0]
    serch_url = start_url + movie_name
    print("开始搜索 ",movie_name," 的字幕")    
    try:
        response1 = requests.get(serch_url, headers=header, timeout = 100)
        #print(response1.status_code)
        #soup1 = BeautifulSoup(response1.text , "html.parser")
        #从搜索结果页面中寻找第一个'/subs/\d+.html',即第一个搜索结果的所有字幕页面链接
        subPage_url = "http://www.zimuku.la" + re.findall(r'/subs/\d+.html',response1.text)[0]
        try:
            response2 = requests.get(subPage_url, headers=header, timeout = 100)
            #print(response2.status_code)
            #在一个电影的所有字幕的页面中寻找每一个字幕对应的详细页面链接，存入列表
            subDetailurlList = re.findall(r'/detail/\d+.html',response2.text)

            #如果字幕总数大于4则只下载下载次数排前四位的
            if len(subDetailurlList)>4:
                soup2 = BeautifulSoup(response2.text, "html.parser")
                #每个字幕对应一个<td  class="tac lang">
                #tdList = soup2.find('td', attrs={'class':'tac lang'})
                #字幕评价和下载次数分别放在两个<td class="tac hidden-xs">标签中,字幕评价是放在标签属性中的，而下载次数放在标签文本中
                #筛选字幕下载次数标签
                tdList = soup2.find_all('td', attrs={'class':'tac hidden-xs'}, string=re.compile('\d+.?\d+万?'))                
                #获取下载次数字符串
                download_times = []
                for td in tdList:                    
                    download_times.append(td.get_text())
                    #print(td.get_text())
                #将下载次数字符串列表转换为整数列表，并统一单位
                times_list = []
                for times_str in download_times:
                    #将x.x万转换为xx000
                    times_str = times_str.replace(".","")
                    times_str = times_str.replace("万","000")
                    #转换为整数类型存入列表，此时列表顺序应该和具体字幕页面的链接顺序一致
                    times_list.append(int(times_str))                
                #筛选下载次数前四大的索引作为下载索引
                download_index = []
                for i in range(4):
                    #保存最大下载次数的列表索引
                    download_index.append(times_list.index(max(times_list)))                    
                    #将最大下载次数置零
                    times_list[times_list.index(max(times_list))] = 0
                Temp_subDetailurlList = []
                for i in download_index:
                    Temp_subDetailurlList.append(subDetailurlList[i])
                #只保留下载次数前四大的detail页面的链接
                subDetailurlList = Temp_subDetailurlList
                #print(subDetailurlList)
                        
            #如果字幕总数小于等于4则全部下载
            for Detailurl in subDetailurlList:
                DetailpageURL = "http://www.zimuku.la" + Detailurl
                try:
                    response3 = requests.get(DetailpageURL, headers=header, timeout = 100)
                    #print(response3.status_code)
                    soup3 = BeautifulSoup(response3.text,"html.parser")
                    Download_url_List.append(soup3.find('a', attrs={'id':'down1'})['href'])
                except:
                    print("detail页面获取失败")
                    traceback.print_exc()
        except:
            print("subs页面获取失败")
            traceback.print_exc() 
    except:
        print("serch页面获取失败")
        traceback.print_exc() 
    

print("共找到",len(Download_url_List),"个字幕文件")
print("下载字幕中...")
default_dir = 'G:\\###字幕'

option = webdriver.ChromeOptions()
#option.set_headless()
option.add_argument('disable-infobars')
#设置Chrome下载文件默认保存文件夹
prefs = {'profile.default_content_settings.popups':0,'download.default_directory':default_dir}
option.add_experimental_option('prefs',prefs)
#隐藏浏览器界面
option.add_argument('--headless')
option.add_argument('--disable-gpu')

driver = webdriver.Chrome(chrome_options = option)
driver.maximize_window()

count = 0
for download_url in Download_url_List:
    try:
        driver.get(download_url)
        sleep(0.5)
        count+=1
        print("下载第",count,"个字幕")
        #查找所有具有相应属性的span标签并点击
        driver.find_element_by_xpath("//span[@class='glyphicon glyphicon-save icon_size']").click()        
        sleep(0.5)
    except:
        print("下载失败")
        traceback.print_exc()

print("字幕下载完毕！")
#driver.quit()

