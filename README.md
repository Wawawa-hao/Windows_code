# Windows_code

### 小Tips

#### 初次运行程序时，如何获取客户区的大小

```c++
static int cxclient, cyclient;

case WM_CREATE:
	RECT rect;
	GetClientRect(hWnd, &rect);
    cxclient = rect.right - rect.left;
    cyclient = rect.bottom - rect.top;
    break;
```

#### 强制刷新客户区，触发WM_PAINT消息

```c++
InvalidateRect(hWnd, 0, true);
```

