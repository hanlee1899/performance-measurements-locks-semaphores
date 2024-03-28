# Lock과 Semaphore에 대한 간단한 성능 측정

* Lock이란?
  
  임계 영역에서의 코드 실행을 원자적으로 돌아가게 하기 위해 하나의 스레드만  
  코드를 실행할 수 있게 만드는 기법<br><br>


* Semaphore란?
  
  Lock을 사용해서 여러 스레드가 임계영역에 대해 접근할 수 있게하는 기능과  
  Condition variable을 사용해서 스레드들의 실행 순서를 관리할 수 있는 기능을  
  하나로 합쳐서 사용할 수 있게 만드는 기법

---

* 목표 :

  특정 자료구조(Double Linked List)를 사용하여 Insert, Delete, Search 스레드들이 하나의 자료구조에 대해서  
  삽입, 삭제, 조회의 작업을 진행하고 모든 작업이 완료되었을 때의 성능 시간을 비교하려고 한다.  
  (임계영역에 대한 코드에 Lock과 Semaphore 두가지 기법을 적용시켜 비교할 것이다.)

---

* 설계 :
  
  - 각 기능별 스레드의 개수
    
    + Insert Thread : 3개
    + Delete Thread : 1개
    + Search Thread : 5개<br><br>

  - 각 Insert Thread들의 역할

    + Insert Thread 01 : 0번에서 9번까지의 노드를 삽입
    + Insert Thread 02 : 10번에서 19번까지의 노드를 삽입
    + Insert Thread 03 : 20번에서 29번까지의 노드를 삽입<br><br>

  - 진행 절차

    Insert Thread 3개는 각자 자신이 맡은 숫자 범위의 노드를 삽입하고  
    Delete Thread와 Search Thread는 모든 노드가 삭제될 때까지 특정노드의 삭제, 조회를 반복한다.  
    (Delete와 Search 스레드들은 seed값이 설정된 rand값으로 특정노드에 대해 작업한다.)  
    삽입된 노드가 없거나 자신이 삭제 또는 조회할 노드가 없다면 해당 스레드는 0.1초 sleep 상태가 된다.  
    모든 작업이 마무리되면 작업의 상태와 사용된 seed값, 총 걸린 시간을 출력하고 프로그램이 종료된다.

---

* 결과 :

  총 3번의 실험을 서로 다른 seed값으로 진행하였고, 각 실험마다 프로그램을 3번씩 실행하였다.  
  측정 시간 평균은 각 실험에서의 3번의 실행에 대한 평균값이다.<br><br>
  
  - 첫번째 실험 결과 : seed값 = 1
    
    | 동기화 기법 | 측정 시간 평균 |
    | :--------: | :-----------: |
    | Lock       | 약 0.024059초 |
    | Semaphore | 약 0.024032초 |
  <br>
  
  - 두번째 실험 결과 : seed값 = 1234
    
    | 동기화 기법 | 측정 시간 평균 |
    | :--------: | :-----------: |
    | Lock       | 약 0.042461초 |
    | Semaphore | 약 0.029439초 |
  <br>

  - 세번째 실험 결과 : seed값 = 541289465
 
    | 동기화 기법 | 측정 시간 평균 |
    | :--------: | :-----------: |
    | Lock       | 약 0.0242초 |
    | Semaphore | 약 0.034초 |