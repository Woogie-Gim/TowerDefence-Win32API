#pragma once

class Object;

// 게임 내의 모든 Object(플레이어, 몬스터, 미사일 등)를 관리하는 싱글턴 매니저
class ObjectManager
{
public:
    DECLARE_SINGLE(ObjectManager);

    ~ObjectManager(); // 소멸자: 전체 Object 정리

    // Object를 매니저에 등록
    void Add(Object* object);
    // Object를 매니저에서 제거 (동시에 메모리 해제)
    void Remove(Object* object);
    // 모든 Object 제거 및 메모리 해제
    void Clear();
    // 프레임 끝에서 한 번에 delete 
    void CollectGarbage();


    // 현재 등록된 Object 목록 반환
    const vector<Object*>& GetObjects() { return _objects; }

    // 템플릿 함수: 지정한 타입의 Object를 생성 및 초기화
    template<typename T>
    T* CreateObject()
    {
        // 타입 체크: T가 Object를 상속받은 타입인지 컴파일 타임에 검사
        static_assert(std::is_convertible_v<T*, Object*>);

        // 동적 할당으로 Object 생성
        T* object = new T();

        // 객체 초기화
        object->Init();

        // 주의: 여기서 Add()를 호출하지 않으면 매니저에 등록되지 않음
        // 설계 의도에 따라 CreateObject에서 Add까지 같이 해주는 게 안전할 수 있음
        return object;
    }

private:
    // 게임 내 존재하는 모든 Object 포인터를 보관하는 컨테이너
    vector<Object*> _objects;
    // 지연 삭제 큐
    vector<Object*> _garbage;
};
