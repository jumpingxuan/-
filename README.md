Prog 1

完成功能:

i.	父行程可以從命令列正確讀取參數

ii.	父行程用fork()產生一個子行程，且模擬過程中只有一個子行程。每個行程執行過程中，亂數 只用給定的亂數種子設定一次。

iii.	shared memory空間只有一塊，而且父行程與子行程只用同一塊 shared memory空間來傳遞正確 的炸射結果給對方。

iv.	父行程與子行程有一個使用POSIX shared memory的同步機制，可以讓父行程與子行程的運作 正確同步。

程式過程:

使用同一塊posix shared memory 空間，利用ptr指向此空間。我是使用ptr[0],這種類似陣列的方式去存取我要傳送給father/child的資料，也藉由shared memory去完成同步的工作。

傳進參數5 10 0與 10 99 0去測後，能夠跑出正確的順序，也能夠判斷是誰贏。
