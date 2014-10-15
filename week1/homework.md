week1 homework
----

간단하게 클라이언트쪽에서 서버와 연결을 맺음,
멀티 스레드를 통한 recv, recv받은 패킷을 게임 스레드로 가져와서 처리하는게 목표.


```C++
struct header{
  int size;
  int id;
};
struct login_request : header{ /* id : 1 */
  char user_id[33];
  char user_pw[33];
};
struct login_response : header{ /* id : 2 */
  int result;
  char user_nickname[33];
}
struct foo : header{ /* id : 3 */
  int a;
};
struct bar : header{ /* id : 4 */
  int a;
};
```

header의 size에는 sizeof(packet)을,
id에는 각 패킷의 id값을 넣어야 함.


요청/응답 예제
```C++
요청 - user_id가 'user' user_pw가 'asdf1234'인 login_request패킷
응답 - result에는 1, user_nickname에 'pjc0247'이 들어있는 login_response패킷

이외의 경우에는 - result가 0
```
```
요청 - a에 5가 들어있는 foo패킷
응답 - 보낸 a값에서 +5된 값이 a에 들어있는 bar패킷
```


서버
____
__주소__ 10.73.45.142
__포트__ 9916
