#pragma once

// 싱글톤 패턴 매크로 선언부
// 클래스 내부에 선언해서 해당 클래스의 유일한 인스턴스를 생성 및 반환함
// 생성자를 private으로 막아 외부에서 new 하지 못하게 제한
#define DECLARE_SINGLE(classname)           \
private:                                    \
    classname() {}                          /* 생성자를 private으로 막음 (외부 생성 금지) */ \
public:                                     \
    static classname* GetInstance()         /* 전역 접근 가능한 유일 인스턴스 반환 함수 */ \
    {                                       \
        static classname s_instance;        /* 프로그램 전체에서 하나만 생성됨 (정적 지역 변수) */ \
        return &s_instance;                 \
    }

// 싱글톤 객체 접근용 매크로
#define GET_SINGLE(classname)   classname::GetInstance()

// 안전한 포인터 삭제 매크로
// delete 후 nullptr로 초기화하여 이중 삭제 방지
#define SAFE_DELETE(ptr)        \
    if (ptr)                    \
    {                           \
        delete ptr;             \
        ptr = nullptr;          \
    }
