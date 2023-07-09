
##Elevator simulator
## Project overview
| Programming language       | Technology           | Graphic technology  |
| -------------------------- |:--------------------:| :------------------:|
| C++                        | WinAPI               | GDI+                |
## Image of the project's window
![Elevator!](https://i.imgur.com/333lOGZ.png "Elevator")
## Description of the image
The picture show the elevator shaft (blue) with 5 levels attached to it (also blue). The levels are numbered from 0 to 4. Inside the shaft there is an elevator cabin (white) that will be the main moving object in this project. Passengers are represented by purple stickmans that will appear on each of the levels as we will press buttons. The overall mass of the passengers (in kilograms) inside the cabin is shown in the top-left corner of the window inside a text box. The number will update on its own.

Each of the buttons creates a new passenger in queue of the corresponding floor. The number labeled on the button symbolises the passenger's target level. The cabin will move automatically simulating the behaviour of a real-life elavator. After staying inactive with empty cabin and no passengers to go for, the cabin will move to level 0 and then wait for further inscructions.

## Notes 
The elevator can only carry up to 600 kilograms of mass. Thus only up to 8 passengers can use the elevator at the same time. The rest will wait in queue on their floors until there are free spots inside the cabin.

## Technical side of the project 

### Classes and objects
To ilustrate our usage of classes and objects we will use passengers or our project as an example.

First we have created a **person** class in the **stdafx.h** file which stores the passenger's mass and numbers **from** which floor **to** which floor they want to move.
```c++
class person
{
public:

	int weight = 70;
	int from = 0;
	int to = 0;
};
```

Then in the main **draw2.cpp** file we have added these variable for storing the objects of a **person** class
```c++
vector <person> prs;
vector <person> prs_elev;
```

Finally, new passengers are created with each press of a button inside a project's window. Here's an extract from the message loop and a fuction that is called
```c++
switch (message)
	{
    case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case F4T3:
			Const(hWnd); //function used for drawings
			floor(hWnd, lvl[3].poz.y, lvl[4].poz.y); // this function will create a new passenger
			break;
     //rest of the code for buttons
    }
     //rest of the code for messages
   }
```
```c++
VOID floor(HWND hWnd, int to, int from)
{
	/*...*/
	person prs_buff;
	prs_buff.from = from;
	prs_buff.to = to;
	prs.push_back(prs_buff);
	/*...*/
}
```

### Drawings
All drawings (stickmans, elevator cabin and shaft, floors) are being drawn using following functions. 
```c++
//examples from the project
graphics.DrawEllipse(&purp, circle); 
graphics.DrawRectangle(&w_pen, elevator.poz.x, elevator.poz.y, 580, 170);
graphics.DrawLine(&b_pen, 600, 480, 600, 500);
//we also invalidate only only the parts of window we need to at the moment using for example
InvalidateRect(hWnd, &elevator_tunnel, TRUE);
```
Some of the drawings are made under the **WM_PAINT** case of the message loop and some are triggered by the buttons.

### Timers
Event like moving the cabin requires the use of timers. Here's an example for our project for the **TIMER_E** responsible for moving the cabin in the current direction.
Handling the timer is done in the messege loop in the switch under the **WM_TIMER** case.
```c++
//start the timer
SetTimer(hWnd, TIMER_E, elevator.velocity, (TIMERPROC)NULL);
//end the timer
KillTimer(hWnd, TIMER_E);
//handling the timer in the message loop
switch (message)
	{
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_E:
			go_to(hWnd, where_to());
			Refresh(hWnd, false);

			if (dir == true)
				elevator.poz.y--; //up
			else
				elevator.poz.y++; //down
			return 0L;
			break;
      //rest of the code for timers
     }
     //rest of the code for messages
   }
```



