#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define IDC_EDIT 101

char currentInput[256] = "";
HWND hEdit;

LRESULT CALLBACK
WindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
    {
    case WM_CREATE:
      {
        hEdit = CreateWindow ("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
                              20, 20, 210, 25, hwnd, (HMENU) IDC_EDIT, NULL, NULL);

        const char *buttons[] =
          {
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "0", "C", "=", "+"
          };

        for (int i = 0; i < 16; i++)
          {
            CreateWindow ("BUTTON", buttons[i],
                          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                          20 + (i % 4) * 55, 60 + (i / 4) * 40, 50, 35,
                          hwnd, (HMENU) (INT_PTR) (200 + i), NULL, NULL);
          }
      }
      break;

    case WM_COMMAND:
      {
        if (LOWORD (wParam) >= 200 && LOWORD (wParam) < 216)
          {
            int index = LOWORD (wParam) - 200;

            const char *labels[] =
              {
                "7", "8", "9", "/",
                "4", "5", "6", "*",
                "1", "2", "3", "-",
                "0", "C", "=", "+"
              };

            const char *label = labels[index];

            if (strcmp (label, "C") == 0)
              {
                currentInput[0] = '\0';
              }
            else if (strcmp (label, "=") == 0)
              {
                double result = 0;
                char op;
                double num1, num2;

                if (sscanf (currentInput, "%lf %c %lf", &num1, &op, &num2) == 3)
                  {
                    switch (op)
                      {
                      case '+': result = num1 + num2; break;
                      case '-': result = num1 - num2; break;
                      case '*': result = num1 * num2; break;
                      case '/': result = (num2 != 0) ? num1 / num2 : 0; break;
                      }

                    snprintf (currentInput, sizeof (currentInput), "%.2f", result);
                  }
                else
                  {
                    strcpy (currentInput, "Error");
                  }
              }
            else
              {
                strcat (currentInput, label);

                if (strchr ("+-*/", label[0]))
                  {
                    strcat (currentInput, " ");
                  }
              }

            SetWindowText (hEdit, currentInput);
          }
      }
      break;

    case WM_DESTROY:
      PostQuitMessage (0);
      break;

    default:
      return DefWindowProc (hwnd, msg, wParam, lParam);
    }

  return 0;
}

int WINAPI
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  WNDCLASS wc = { 0 };
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = "CalcWindow";
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);

  RegisterClass (&wc);

  HWND hwnd = CreateWindow ("CalcWindow", "Калькулятор",
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            260, 300,
                            NULL, NULL, hInstance, NULL);

  ShowWindow (hwnd, nCmdShow);

  MSG msg;
  while (GetMessage (&msg, NULL, 0, 0))
    {
      TranslateMessage (&msg);
      DispatchMessage (&msg);
    }

  return 0;
}
