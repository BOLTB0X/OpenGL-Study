# OpenGL Study

![아키라메나이데](https://github.com/BOLTB0X/OpenGL-Tutorial/blob/main/Img/%EC%95%84%ED%82%A4%EB%9D%BC%EB%A9%94%EB%82%98%EC%9D%B4%EB%8D%B0.gif?raw=true)

## OpenGL

<details>
<summary> OpenGL 이란?</summary>

> OpenGL is mainly considered an API (Application Programming Interface) that provides us with a large set of functions that we can use to manipulate graphics and images

<br/>

<p align="center">
   <img src="https://learnopengl.com/img/getting-started/opengl.jpg" alt="Example Image" width="80%">
</p>

<br/>

- OpenGL은 **Open Graphics Library** 약자, 2D 및 3D 그래픽을 렌더링하는 데 사용되는 크로스플랫폼 **그래픽 API**
   - OS에서 OpenGL 인터페이스를 제공

   - GPU는 **OpenGL implementation**(그래픽 카드 드라이버)를 제공

   - Windows에서는 **opengl32.dll** 제공

- 주로 게임, CAD, 시뮬레이션 및 시각적 데이터 처리 애플리케이션에서 사용

- Windows, OS X, Linux, iOS, Android 등 OS에 구제받지 않고 동작 가능

- 상태 기반(State Machine) API

   - 명령어가 상태를 변경하며, 특정 상태에서만 특정 연산이 가능

- GPU를 활용하여 빠른 그래픽 연산 수행

---

</details>

## Intro

<details>
<summary> OpenGL 은 어떻게 동작되는 가? </summary>

### 돌아가는 방식

- 직접 실행되는 **Library** 가 아니라 그래픽 드라이버를 통해 GPU와 상호작용하는 **API**

   <details>
   <summary> Library vs API </summary>

   - cf. *Library*
      > Collection of functions, pre-compiled routines or reusable components of code
   
      이미 구현된 코드 모음

      - 재사용 가능한 코드 묶음

      - 함수, 클래스, 모듈 등의 집합

      - 그래픽 처리, 수학 연산 등 같이 특정 기능에 초점

      - 개발자가 직접 호출하여 사용

      <br/>

   - cf. *API (Application Programming Interface)*
      > Collection of software protocols

      어떤 SW가 다른 SW와 상호작용할 수 있도록 제공하는 명령어 및 규칙의 집합
   
      Library와 달리 API는 반드시 코드로 구현된 것 X, 단순히 인터페이스의 정의만 제공 가능

      - 특정 기능을 제공하는 인터페이스

      - 개발자가 직접 구현하는 것이 아니라, 제공된 방식을 따라 사용

      - 외부 시스템을 호출하거나, 라이브러리를 통해 제공됨
   
      - SW 간 상호작용을 가능하게 함

      <br/>

   </details>


1. 애플리케이션 -> OpenGL API 호출

2. OpenGL -> 그래픽 드라이버 (GPU에 명령 전달)

3. GPU -> 렌더링 수행

4. 프레임버퍼 -> 화면 출력

</details>

<details>
<summary> 기본 구성 </summary>

1. **Context**

   - OpenGL을 사용하기 위해서는 반드시 **Context**를 생성해야 함
   
   - OpenGL의 상태 정보를 저장하며, OpenGL 함수 호출 시 **Context**가 사용됨

   ---

2. **Vertex** && **Fragment**

   <p align="center">
      <img src="https://learnopengl.com/img/getting-started/pipeline.png" alt="Example Image" width="80%">
      <br/>
      이미지 출처: learnopengl.com
   </p>

   - 버텍스(Vertex)

       3D 객체의 점 데이터를 의미, `glDrawArrays` 등을 사용해 렌더링함

   - 프래그먼트(Fragment)
      
      래스터화 후 개별 픽셀에 적용되는 데이터로, 최종 화면에 출력되는 색상 결정

      ---

3. **VAO** , **VBO** , **EBO**

   <p align="center">
      <img src="https://learnopengl.com/img/getting-started/vertex_array_objects.png" alt="Example Image" width="70%">
      <br/>
      이미지 출처: learnopengl.com
   </p>

   - VAO(Vertex Array Object)
   
     버텍스 속성(Attributes)을 저장하는 객체

   - VBO(Vertex Buffer Object)
   
     버텍스 데이터를 GPU에 저장하는 버퍼

   <p align="center">
      <img src="https://learnopengl.com/img/getting-started/vertex_array_objects_ebo.png" alt="Example Image" width="70%">
      <br/>
      이미지 출처: learnopengl.com
   </p>

   - EBO(Element Buffer Object)
   
      삼각형을 효율적으로 그리기 위한 인덱스 버퍼

      ---

4. **Shader**

   - GPU에서 실행되는 작은 프로그램 , 렌더링 과정을 제어

   - *Vertex Shader*

       *버텍스를 변환하는 역할*

   - *Fragment Shader* 
   
      *픽셀 단위로 색상을 결정*

   ---

</details>


<details>
<summary> 렌더링 파이프라인 </summary>

```
[CPU 코드]
   ↓
[정점 데이터 (VBO)]
   ↓ 설명
[정점 구조 (VAO)]
   ↓ 전달
[Vertex Shader]
   ↓
[도형 조립]
   ↓
[Rasterizer]
   ↓
[Fragment Shader]
   ↓
[Framebuffer (결과)]
```

1. CPU(C++)에서 정점 데이터를 준비하고 VBO에 넣음

2. **VAO**로 정점 속성 포맷을 정의

3. **GPU**에서 **Vertex Shader** 실행(각 정점마다 위치 계산)

4. **Primitive Assembly(도형 조립)** – 삼각형 등의 기본 도형 생성

5. **Rasterization** – 삼각형 -> 픽셀로 분해 (fragment)

6. **Fragment Shader** 실행(픽셀 색상 결정)

7. 화면(FrameBuffer)에 출력

자세한 것은 [렌더링 파이프라인 TODO]() 참조

</details>

## Draw

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EC%9B%90%EA%B7%B8%EB%A6%AC%EA%B8%B0.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B5%AC%EA%B7%B8%EB%A6%AC%EA%B8%B0.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Circle
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Sphere
      </p>
      </td>
    </tr>
  </table>
</p>

## Tutorial

- [Get Started](https://github.com/BOLTB0X/OpenGL-Study/tree/main/VisualStudioVer)

## 참고

- [learn opengl](https://learnopengl.com/)

- [티스토리-혼자 하는 코딩([OpenGL] OpenGL이란?)](https://gofo-coding.tistory.com/entry/OpenGL#title-0)

- [rapidapi-블로그-API vs Library (What’s the Difference?) By Team RapidAPI](https://rapidapi.com/blog/api-vs-library/)

- [티스토리-꽉퓨타의 SW 이야기(OpenGL - 윈도우에 개발환경 구축하기 (GLFW, GLAD 사용))](https://kwak-story.tistory.com/3)

- [Velog-tejava7177.log([OpenGL] Xcode에서 OpenGL)](https://velog.io/@tejava7177/OpenGL-Xcode%EC%97%90%EC%84%9C-OpenGL)
