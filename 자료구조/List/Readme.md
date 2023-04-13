# 리스트 ADT
원소는 원소 앞의 원소 개수(rank)를 특정함으로써 접근

* 일반
    - boolean isEmpty()
    -   integer size()
    -   iterator elements()
*   접근
    - element get(r)
*   갱신
    -   element set(r,e)
    -   add(r,e)
    -   addFirst(e)
    -   addLast(e)
    -   element remove(r)
    -   element removeFirst()
    -   element removeLast()
* 예외
    - invalidRankException()
    - fullListException()
    - emptyListException()