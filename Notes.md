# 1. Windows错误处理

```cpp
DWORD WINAPI FormatMessage(
  __in      DWORD dwFlags,
  __in_opt  LPCVOID lpSource,
  __in      DWORD dwMessageId,
  __in      DWORD dwLanguageId,
  __out     LPTSTR lpBuffer,
  __in      DWORD 
nSize,
  __in_opt  va_list *Arguments
);

获取消息字符串。

```



| **Value**                                   | **Meaning**                                                  |
| ------------------------------------------- | ------------------------------------------------------------ |
| FORMAT_MESSAGE_ALLOCATE_BUFFER   0x00000100 | 函数自动使用LocalAlloc函数，来为lpBuffer分配内存。lpBuffer必须是一个指向LPTSTR的指针，同时，你必须对这个指针进行下面的强制转换(LPTSTR)&lpBuffer。此时，nSize规定了，FormatMessage函数，至少要分配nSize*sizeof(TCHAR)个自己的内存。如果要释放这段分配的内存，请使用LocalFree函数。 |
| FORMAT_MESSAGE_ARGUMENT_ARRAY   0x00002000  | 这个标志表示，Arguments参数不是一个va_list结构体，而是一个指向数组的指针，这个数组中的值就是字符串中的参数。  如果使用这个标志，数组中的值必须是32为的，不能是64位的，如果要使用64位的值，那么，必须使用va_list结构体。 |
| FORMAT_MESSAGE_FROM_HMODULE   0x00000800    | lpSource参数，指向一个模块句柄，如果lpSource=NULL，那么代表当前应用程序映像。这个表示不能和FORMAT_MESSAGE_FROM_STRING标志一起使用。  如果lpSource指向的模块，没有消息列表资源，那么FormatMessage函数返回一个错误：ERROR_RESOURCE_TYPE_NOT_FOUND。 |
| FORMAT_MESSAGE_FROM_STRING   0x00000400     | lpSource指向一个NULL结尾的字符串，这个字符串就是消息。这个标志不能和FORMAT_MESSAGE_FROM_HMODULE 和 FORMAT_MESSAGE_FROM_SYSTEM一起使用 |
| FORMAT_MESSAGE_FROM_SYSTEM   0x00001000     | 这个函数从系统消息列表资源中，获取消息。  如果这个标志被设置，那么可以讲GetLastError函数的返回值，作为参数传给给FormatMessage函数，来获取错误提示。 |
| FORMAT_MESSAGE_IGNORE_INSERTS   0x00000200  | 这个标志表示Arguments参数将被忽略。                          |

*lpSource* [in, optional] 

消息所在位置，这个参数类型，根据dwFlags标志来设定。

*dwMessageId* [in]

消息索引，如果lpSource是一个字符串，那么这个参数被忽略。

*dwLanguageId* [in]

语言标识，在这里使用NULL，表示操作系统的语言。

*lpBuffer* [out] 

接受消息字符串的内存块。

*nSize* [in]

内存大小。如果内存由FormatMessage函数分配，那么这个参数表示内存块的最小大小。

*Arguments* [in, optional] 

消息中的参数。

**Return Value**

如果函数成功，返回字符消息的字符数。

如果函数失败，返回0，用GetLastError函数，获取错误信息。

 

 

**错误代码各个位数的意义：**GetLastError**函数返回值！**SetLastError**可是设置这个错误代码。

| **位**   | **31-30**                                          | **29**                             | **28**   | **27-16**                   | **15-0**                     |
| -------- | -------------------------------------------------- | ---------------------------------- | -------- | --------------------------- | ---------------------------- |
| **内容** | **严重性**                                         | **Micorsoft/**  **客户定制**       | **保留** | **Facility**  **代码**      | **异常代码**                 |
| **含义** | **0=**成功  **1=**提示信息  **2=**警告  **3=**错误 | **0=**微软定义  **1=**客户定义代码 | **0**    | **前**256**个值有微软保留** | **Micorsoft/**  **客户定制** |

 

Windows函数返回值的意义：

**VOID**：这个函数不可能失败！

**BOOL**：FALSE失败；TRUE成功。

**HANDLE**：失败返回NULL，否则返回非零句柄。如果有特殊说明，则可能为特殊值例如：INVALID_HANDLE_VALUE。

**PVOID**：返回一个内存地址，失败为NULL

**LONG/DWORD**：应该根据SDK说明来确定函数状况。

==windows 跟linux下最复杂的函数CreateFile==

# 2. unicode 字符

* 字符编码：

**UTF-8**：一个字节一个字符，有些字符是2个字节，有的字符是3个字节，还有的字符是4个字节。

**UTF-16**：大部分字符都是2个字节。有部分字符是4个字节的。这个就是unicode字符

**UTF-32**：所有字符都是4个字节。

* 字符的数据类型：

UNICODE类型的字符都是宽字符，在C中的数据类型是：wchar_t。在高版本vc中，这是一个基本数据类型！！！

==一个宏，可以兼容UNICODE和ASCII字符，这个宏就是TEXT，数据类型用TCHAR！！！==

```shell
TCHAR 在定义了UNICODE宏的时候，表示wchar_t,如果没有定义UNICODE宏那么TCHAR就代表char，windows.h头文件里有

#define UNICODE这个宏
```

**在控制台中不要使用unicode字符，会出问题**

* 在windows中，有UNICODE类型的函数和ASCII类型的函数，例如CreateWindowEx函数。

在WinUser.h中，有如下定义：

```cpp
#ifdef UNICODE

#define CreateWindowEx CreateWindowExW

#else

#define CreateWindowEx CreateWindowExA

#endif // !UNICODE

 
```

在windows98的时候，

```cpp
CreateWindowW()

{

  UNICODE_TO_ASCII()

  return CreateWindowA()

}
```



 在windows2000以后呢，正好相反： 

```cpp
CreateWindowA()

{

  ASCII_TO_UNICODE ()

  return CreateWindowW()

}
```

也就是，在我们的程序中，如果定义了UNICODE，那么我们使用的是宽字符，否则，使用的是ASCII类型的函数，所有我们统一使用==CreateWindowEx==函数，让系统区分是否使用宽字符或者ASCII字符类型的函数。这样程序的兼容性更好。 

创建自己的动态链接库的时候，可以使用这种方法！！！

* 一些C运行库函数，没有自己的ASCII版和UNICODE版，如何区分。

例如strlen：

| **TCHAR.H routine** | **_UNICODE &  _MBCS not defined** | **_MBCS defined** | **_UNICODE defined** |
| ------------------- | --------------------------------- | ----------------- | -------------------- |
| **_tcslen**         | **strlen**                        | **strlen**        | **wcslen**           |
| **_tcsclen**        | **strlen**                        | **_mbslen**       | **wcslen**           |
| **_tcsclen_l**      | **strlen_l**                      | **_mbslen_l**     | **wcslen_l**         |

 

通过MSDN，或者VC2010的帮助，可以获得对应的TCHAR.h版函数，我们尽量使用TCHAR.h中的函数，这样我们就可以不考虑我们当前程序是否是UNICODE或者ASCII版。

* 我们编程的时候，尽量使用安全字符串，例如strcpy就是一个非安全函数，当你再程序中使用这个函数的时候，你就会发现，编译器会出现警告，同时给出建议，请遵守。

编译器会提示我们使用strcpy_s函数，此时，我们可以查找这个函数，并找到这个函数的TCHAR.h版。

| **TCHAR.H routine** | **_UNICODE &  _MBCS not defined** | **_MBCS defined** | **_UNICODE defined** |
| ------------------- | --------------------------------- | ----------------- | -------------------- |
| **_tcscpy_s**       | **strcpy_s**                      | **_mbscpy_s**     | **wcscpy_s**         |

* 几个有用的StringSafe.h函数：

```cpp
HRESULT StringCchCat(

__inout LPTSTR pszDest,

__in   size_t cchDest,

__in   LPCTSTR pszSrc

);

 

pszDest：目标字符串；

cchDest：目标字符串内存大小；

pszSrc：  源字符串。
```



这是一个字符串拷贝函数，在strsafe.h中。

 

**HRESULT StringCchPrintf(**

 **__out LPTSTR pszDest,**

 **__in  size_t cchDest,//****目标内存大小**

 **__in  LPCTSTR pszFormat,**

 **__in  ...**

**);**

这个函数和printf函数用法类似，只是将字符写入了pszDest内存块中，而不是屏幕上！

 

这两个函数如果成功的话，返回：S_OK，

STRSAFE_E_INVALID_PARAMETER：传入了无效参数。

STRSAFE_E_INSUFFICIENF_BUFFER：目标字符串内存的大小不够。

* 两个字符串比较函数：

```cpp
int CompareString(
  __in  LCID Locale,
  __in  DWORD dwCmpFlags,
  __in  LPCTSTR lpString1,
  __in  int cchCount1,
  __in  LPCTSTR lpString2,
  __in  int cchCount2
);
int CompareStringOrdinal(
  __in  LPCWSTR lpString1,
  __in  int cchCount1,
  __in  LPCWSTR lpString2,
  __in  int cchCount2,
  __in  BOOL bIgnoreCase
);

```

* 多款字符和ASCII字符之间的转换函数！！

  ```cpp
  int MultiByteToWideChar(
    __in   UINT CodePage,
    __in   DWORD dwFlags,
    __in   LPCSTR lpMultiByteStr,
    __in   int cbMultiByte,
    __out  LPWSTR lpWideCharStr,
    __in   int cchWideChar
  );
  
  int WideCharToMultiByte(
    __in   UINT CodePage,
    __in   DWORD dwFlags,
    __in   LPCWSTR lpWideCharStr,
    __in   int cchWideChar,
    __out  LPSTR lpMultiByteStr,
    __in   int cbMultiByte,
    __in   LPCSTR lpDefaultChar,
    __out  LPBOOL lpUsedDefaultChar
  );
  ```

* 判断字符串是不是unicode字符串

  这个函数很好用，但是不能保证百分之百正确

  ```cpp
  BOOL IsTextUnicode(
    __in     const VOID *lpv,
    __in     int iSize,
    __inout  LPINT lpiResult
  );
  ```

  

# 3. windows内核对象

### 3.1 基本知识

