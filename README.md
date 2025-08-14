# Win32 API 기반 타워 디펜스 미니게임
- GDI+와 **Win32 API**를 활용해 구현한 2D 타워 디펜스 게임입니다.
  - C++ 언어로 직접 게임 루프, 렌더링 오브젝트 매니저, 시간 관리 등을 구현하여 상용 엔진 없이 동작하는 **순수 Win32 기반 게임 구조**를 설계 했습니다.

![alt text](<Assets/TowerDefence Main.png>)

## 🎮 프로젝트 개요

- 개발 기간 : 2025.08.07 ~
- 개발 환경
  - 언어 : C++17
  - API : Win32 API, GDI+
  - IDE : Visual Studio 2022

  ## 🛠 주요 기능

### 1. 게임 루프 & 시간 관리
- `TimeManager`를 통해 **deltaTime 기반 프레임 독립적 동작** 구현
- 초당 FPS 측정 및 디버그 출력 가능

### 2. 오브젝트 매니저
- `ObjectManager`로 게임 내 모든 객체의 생성, 업데이트, 삭제를 통합 관리
- `GameObjectType` Enum을 통한 타입 분류 (타워, 몬스터, 미사일 등)

### 3. 타워 시스템
- 출현하는 적의 순서에 따라 공격 순서 결정
- 적의 type을 인식하여 유도 탄환 발사
- 단일샷, 트리플샷 타워 2종 구현

### 4. 미사일 시스템
- 직진/유도 모드 지원 (`SetHomingEnabled`)
- GDI+ 스프라이트 렌더링 지원
- 몬스터 충돌 판정 및 제거 처리

### 5. 몬스터 AI
- 고정된 경로를 따라 이동
- HP 및 피격 처리 (`OnHit` → 사라짐)

### 6. 렌더링
- **더블 버퍼링** 구현으로 깜빡임 방지
- GDI+를 이용한 PNG 스프라이트 출력
- 스프라이트 미적용 시 기본 도형으로 대체

## 📂 프로젝트 구조

TowerDefence/
├── 00. MainGame/
│   ├── Game.cpp
│   ├── Game.h
│   ├── TowerDefence.cpp
├── 01. Managers/
│   ├── InputManager.cpp
│   ├── InputManager.h
│   ├── ObjectManager.cpp
│   ├── ObjectManager.h
│   ├── SceneManager.cpp
│   ├── SceneManager.h
│   ├── TimeManager.cpp
│   ├── TimeManager.h
├── 02. Scene/
│   ├── Game/
│   │   ├── Scene.cpp
│   │   ├── Scene.h
├── 03. Objects/
│   ├── Game/
│   │   ├── EnemySpawner.cpp
│   │   ├── EnemySpawner.h
│   │   ├── Map.cpp
│   │   ├── Map.h
│   │   ├── Missile.cpp
│   │   ├── Missile.h
│   │   ├── Monster.cpp
│   │   ├── Monster.h
│   │   ├── Player.cpp
│   │   ├── Player.h
│   │   ├── Player1.cpp
│   │   ├── Player1.h
│   ├── Object.cpp
│   ├── Object.h
├── 04. Resources/
│   ├── BGM/
│   ├── UI/
│   │   ├── LifeUI.h
│   │   ├── ResourceBase.cpp
│   │   ├── ResourceBase.h
├── 99. Headers/
│   ├── Default/
│   ├── Defines.h
│   ├── Enums.h
│   ├── GdiPlusSafe.h
│   ├── pch.cpp
│   ├── pch.h
│   ├── Types.h
│   ├── Utils.cpp
│   ├── Utils.h


## 🚀 실행 방법

1. **빌드 환경 준비**
   - Visual Studio 2022 이상
   - Windows 환경 (Win32 API 지원 필수)

2. **실행 파일 위치**
   - `x64/Debug/타워디펜스.exe`

3. **리소스 경로**
   - `Assets` 폴더를 실행 파일과 동일한 경로에 두어야 함

## 📸 스크린샷 / GIF

![alt text](Assets/Monster.gif)

![alt text](Assets/Shot.gif)

![alt text](<Assets/Triple Shot.gif>)

![alt text](Assets/Life.gif)

## 업데이트 예정 사항
- 적 생명력, 미사일 밸런스 조정
- 스테이지 클리어 조건 구현
- 타워 설치 UI 및 쿨타임 구현
- 게임 오버 UI 및 재시작 구현
- 메인 화면 구현
- 재화 시스템 구현

## 📜 라이선스
본 프로젝트는 **포트폴리오 용도**로만 사용됩니다.
