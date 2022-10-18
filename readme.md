# Fusion

## Inference

This library provides an implementation of Fusion which is a maliciously secure inference protocol resilient malicious servers (NDSS 2023 accpected paper: Fusion: Efficient and Secure Inference Resilient to Malicious Servers).


## How To Run

In order to determine the appropriate R and B, we use Test.py to search optimized R and B. 

We used a four-layer DNN network and a ResNet50 network to implemente experiments of secure inference.
The source code of the used networks is included in the Networks folder, along with specific implementation details.

We implement two different model extraction methods to steal the model parameters of the server through attack 1 and attack 2 respectively, and then test the accuracy of stolen model to check the effectiveness of the defense method that Fusion integrated with.

The source code of the two attacks (attack 1 and attack 2) used in Fusion is shown in the attack folder.

## Reference papers

We thank the authors of the following papers for open-sourcing the code.

[Cheetah: Lean and Fast Secure Two-Party Deep Neural Network Inference](https://eprint.iacr.org/2022/207)

Zhicong Huang, Wen-jie Lu, Cheng Hong, and Jiansheng Ding

*USENIX Security 2022*



[CrypTFlow2: Practical 2-Party Secure Inference](https://eprint.iacr.org/2020/1002)  

Deevashwer Rathee, Mayank Rathee, Nishant Kumar, Nishanth Chandran, Divya Gupta, Aseem Rastogi, Rahul Sharma 

*ACM CCS 2020*



[Delphi: A Cryptographic Inference Service for Neural Networks](https://eprint.iacr.org/2020/050.pdf)   

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
