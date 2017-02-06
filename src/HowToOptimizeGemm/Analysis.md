

HPLA

Kaleb Bodisch


references https://www.ibm.com/support/knowledgecenter/SSLTBW_2.2.0/com.ibm.zos.v2r2.cbcpx01/atlasexample1.htm

http://www.netlib.org/lapack/explore-html/index.html

https://www.math.utah.edu/software/lapack/lapack-blas/dgemm.html

Part 1)

Reference Code:
       774,524,626      cache-misses                                                
    69,097,696,246      instructions

4x4_10:

       598,572,733      cache-misses                                                
    38,010,592,058      instructions                                                


dgemm:
       452,037,532      cache-misses                                                
    48,088,122,925      instructions 



As we can see the number of cache-misses is less in the blas implementation than the 4x4_10 implementation but there are more instructions. This results from dgemm having better locality. The reason why there are more instructions for the dgemm version may be due to the fact that 4x4_10 directly uses vector registries and instructions while dgemm may have overhead when creating machine instructions. 

Attached to this repository is also a comparative png plot of 4x4_10 vs dgem implementation. From this plot see that dgemm actually runs at a slower frequency than the 4x4 version. 


Part 2)

MMult0

     1,382,946,609      cache-misses                                                
    69,137,947,378      instructions 



MMult0_symm


     1,789,827,929      cache-misses                                                
    57,989,691,696      instructions                                                






From the above, we can see that the symmetric multiplication required less instructions to be issued, while there actually ended up being more cache-misses during execuction. Since there were less loops to iterate over in the symmetric implimitation, it makes sense that less instructions are issued. There may be more cache-misses in symmetric due to the fact that C and more specifically B may be getting reloaded more times than neccesary. While conserving locality of A in this function, I believe that this only hinders data fetching from matrix B. As a result, the symmetric matrix multiplication also has a slower frequency than the generic MMult0 implimitation. 

