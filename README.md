## fdisk_Imitation

Write a program similar to `fdisk -l`.

#### Compilation command:
```
$ gcc -o mydisk mydisk.c sources/gpt_parsing.c sources/mbr_parsing.c sources/unit_conversion.c
```

#### Exemples of the output :
---
### MBR
---
```
$  fdisk -l ./myhdd_mbr
```

![1](https://user-images.githubusercontent.com/89909599/199345563-5e18ca6e-6565-4e47-a4d2-bc4da44bd7d3.png)
```
$ ./mydisk ./myhdd_mbr
```
![2](https://user-images.githubusercontent.com/89909599/199345598-594dc33a-c934-4345-86f0-a738c06a69ee.png)

---
### GPT
---
```
$  fdisk -l ./myhdd_gpt
```
![11](https://github.com/Emna-Rekik/Test/assets/89909599/98bbd0c0-9d80-4453-807d-0938760df618)


```
$ ./mydisk ./myhdd_gpt
```
![22](https://github.com/Emna-Rekik/Test/assets/89909599/2a774487-4783-483d-aa80-f7ed8c1f598b)
