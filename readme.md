# Fusion

## Inference

This library provides a more secure solution to machine learning about malicious security.

We test the deep learning aspect by implementing a four-layer DNN network and ResNet-50, here we mainly implement the four-layer DNN training and prediction function.

We predict the data set by T public samples and R*B request samples, and then we get the result labels of the recorded predicted samples, so that the samples themselves can be compared with the predicted results by the main function to do a sample read and comparison functions. 

## How To Run

In order to determine the values of R and B, we test the two values and determine the final data to be used by Test.py. 


We implement two different model extraction methods to steal the parameters of the trained model through extract1 and extract2 respectively, and then test whether the defense methods are effective.

The source code of the four frameworks is included in the Networks folder, along with specific implementation details of the 4-layer DNN. The source code of the two attacks used in the article is shown in the attack folder

## Reference paper

[Cheetah: Lean and Fast Secure Two-Party Deep Neural Network Inference](https://eprint.iacr.org/2022/207)

Zhicong Huang, Wen-jie Lu, Cheng Hong, and Jiansheng Ding

*USENIX Security 2022*



[CrypTFlow2: Practical 2-Party Secure Inference](https://eprint.iacr.org/2020/1002)  

Deevashwer Rathee, Mayank Rathee, Nishant Kumar, Nishanth Chandran, Divya Gupta, Aseem Rastogi, Rahul Sharma 

*ACM CCS 2020*



[Delphi: A Cryptographic Inference Service for Neural Networks_](https://eprint.iacr.org/2020/050.pdf)   

Pratyush Mishra, Ryan Lehmkuhl, Akshayaram Srinivasan, Wenting Zheng, Raluca Ada Popa

*USENIX Security 2020*

[A Framework for Efficient Mixed-Protocol Secure Two-Party Computation](https://encrypto.de/papers/DSZ15.pdf)

Daniel Demmler, Thomas Schneider and Michael Zohner 

*NDSS 2015*



[Practical Black-Box Attacks against Machine Learning](https://arxiv.org/pdf/1602.02697v4.pdf)

Nicolas Papernot, Patrick McDaniel, Ian Goodfellow, Somesh Jha, Z. Berkay Celik, Ananthram Swami 

*AsiaCCS 2017*



[Cryptanalytic Extraction of Neural Network Models](https://link.springer.com/chapter/10.1007/978-3-030-56877-1_7)

Nicholas Carlini, Matthew Jagielski, Ilya Mironov

*CRYPTO 2020*
