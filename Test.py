#!usr/bin/python
# -*- coding: utf-8 -*-
from scipy.special import comb, perm
def optimize():    #定义最优化函数
#建空列表收集B,T,Pr的值
    Pr_list = []
    B_list = []
    T_list = []
    R_list = []
    T = 200;
#    R = 2 ** (7);
    B = 7;
    for R in range(1,100,1):
    #for T in range(0,i,1):
        # 近似Pr
        #Pr = R * ((B / (R * B + T)) ** B)
        # 不近似Pr
        Pr = R / (comb(( R * B + T ),B))
        print("Pr=", Pr)
        Pr_list.append(Pr)
        B_list.append(B)
        T_list.append(T)
        R_list.append(R)


    return Pr_list,B_list,T_list,R_list

Pr_list,B_list,T_list,R_list = optimize()
#使用python自带的min()函数，找出Pr_value这个列表中最小的值，最小值为0
for Pr in Pr_list:
    if Pr <= 2 ** (-40):
        index = Pr_list.index(Pr)
        B_value = B_list[index]
        T_value = T_list[index]
        R_value = R_list[index]
        print("Pr=", Pr, "B=", B_value,"T=", T_value, "R=", R_value)


