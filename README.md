## fdisk_Imitation

Write a program similar to ``` fdisk -l ```

#### Compilation command :

```
$ gcc sources/unit_conversion.c sources/gpt_parsing.c sources/mbr_parsing.c  mydisk.c -o mydisk
```
This program supports mbr and gpt parsing.

#### Examples of the output :

---
#### MBR
---

```
$ fdisk -l ./myhdd_mbr
```
![1](https://user-images.githubusercontent.com/89909599/199345563-5e18ca6e-6565-4e47-a4d2-bc4da44bd7d3.png)

```
$ ./mydisk ./myhdd_mbr
```
![2](https://user-images.githubusercontent.com/89909599/199345598-594dc33a-c934-4345-86f0-a738c06a69ee.png)

---
#### GPT
---

```
$ fdisk -l ./myhdd_gpt
```
![11](https://user-images.githubusercontent.com/89909599/199847545-5805fd03-0371-4f5d-86fc-f5269cd9005c.png)

```
$ ./mydisk ./myhdd_gpt
```
![22](https://user-images.githubusercontent.com/89909599/199847581-0d3b02de-892e-47a5-a61d-b48b63065915.png)

