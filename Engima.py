# -*- coding: utf-8 -*-
import random
from copy import copy
reflector=[18, 16, 12, 15, 19, 13, 23, 20, 9, 8, 21, 14, 2, 5, 11, 3, 1, 22, 0, 4, 7, 10, 17, 6, 25, 24]
raw_cog=[[6, 10, 23, 9, 19, 2, 21, 1, 7, 24, 0, 17, 15, 3, 8, 4, 14, 12, 16, 25, 11, 18, 22, 20, 13, 5]
,[0, 7, 1, 10, 19, 5, 9, 18, 4, 23, 2, 20, 15, 24, 22, 8, 12, 25, 6, 13, 3, 11, 21, 17, 16, 14]
,[23, 2, 4, 13, 14, 21, 12, 0, 9, 16, 5, 17, 20, 22, 10, 24, 7, 3, 25, 8, 15, 6, 19, 18, 11, 1]]

def shift(List,n):
    return List[n:]+List[:n]

class Cog:#模拟转子的类
    def __init__(self,temp):
        self.cog=copy(temp)
    def rotate(self):
        #self.cog=self.cog[1:]+self.cog[:1]
        self.cog=shift(self.cog,1)
    def printStatus(self):#这个只是用来输出转子当前的状态
        for i in self.cog:
            print i
    def nextNum(self,x):
        return self.cog[x]
    def lastNum(self,x):
        return self.cog.index(x)

class Enigma:
    def __init__(self,cogSum):#cogSum是要使用的转子树
        global reflector
        self.lenCipher=0 #这个记录已经密文的长度，在后面计算转子转动时有用
        self.cogSum=cogSum
        self.cogList=[] #所有使用的转子list
        self.reflector=copy(reflector) #反射器
        for i in range(0,cogSum):
            self.cogList.append(Cog(raw_cog[i]))
    def printStatus(self):#输出Enigma密码机当前的状态
        for i in range(0,self.cogSum):
            print 'cog'+str(self.cogSum)+': '
            self.cogList[i].printStatus()
            print
        print 'reflector:',
        for i in self.reflector:
            print i,
    def dealChar(self,c):#处理一个字符的加密，并且转动转子
        #加密字符
        num=ord(c)-ord('a')
        for i in range(0,self.cogSum):
            num=self.cogList[i].nextNum(num)
        num=self.reflector[num]
        for i in range(self.cogSum-1,-1,-1):
            num=self.cogList[i].lastNum(num)
        #转动转子
        for i in range(0,self.cogSum):
            if 26**i<=self.lenCipher and self.lenCipher%26**i==0:
                self.cogList[i].rotate()
        return chr(ord('a')+num)
    def encode(self,oriStr): #处理一个字符串的加密，这里只处理小写英文字母，忽略其他字符
        ciphertext=''
        oriStr=oriStr.lower()
        for eachChar in oriStr:
            if eachChar.isalpha():
                self.lenCipher+=1
                ciphertext+=self.dealChar(eachChar)
        return ciphertext

def main():
    sumcog=input('请输入要使用的转子数目（最多3个）：')
    for i in range(0,sumcog):
        print '请输入一个0-25的数字以确定第'+str(i+1)+'个转子起始位置：'
        c1=input()
        raw_cog[i]=shift(raw_cog[i],c1) #调整转子的初始位子

    rawString=raw_input('请输入要加密的字符串')
    test=Enigma(sumcog)
    temp=test.encode(rawString)
    print temp

if __name__ == '__main__':
    main()
