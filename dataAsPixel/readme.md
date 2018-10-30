# Usage:

Encode: data -> png  
```shell  
 ./dataAsPixel.py
 Decode [Y/n]? n
 Encode: myDataToPng.txt
 Created result.png
```  

Decode: png -> data
```shell  
 ./dataAsPixel.py
 Decode [Y/n]? y
 Decode: result.png
 Created result.decode
```  

Bugs:
- A lot of \0 at the end of the file if the size of the data is not a square number