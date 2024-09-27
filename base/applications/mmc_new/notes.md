# Observations (of MMC 3.0...)

When an MDI window is open with it's root node as a snap-in which is removed
from the workspace, the window is closed.

You can not add or remove snap-ins when any snap-in's dialog box is open.

MMC always prompts to save changes to the console when you close the last MDI

## Size on a 3840x2160 screen:

Loads with frame window size:
- With taskbar on screen: 2880x1523
- Without taskbar on screen: 2880x1577
- MDI Client window maximized width: 2856
- Scope Pane Treeview width: 588
- Result Pane Listview width: 1665
- Actions Pane (N/A for MMC 2.0) width: 588

So.. Scope pane width = initial window width / 4.9?

## Menu Items

Without any console loaded:

 - Title displays "Microsoft Management Console"
 - Menu:

```
File:
    New\tCtrl+N
    Open...\tCtrl+O
    SEPARATOR
    (list of recently opened files, see Registry. If there isn't any recent
    files stored, just display a greyed out menu item "Recent File")
    SEPARATOR
    Exit
    
Help:
    Help Topics
    TechCenter Web Site
    SEPARATOR
    About Microsoft Management Console...
```
    

With a console loaded:

- Title of MMCMainFrame displays the name of the console
- Icon for MMCMainFrame changes to the icon which is set for the console
- Icon for all MDI child windows have the icon which is set for the console
- Title of MDI child windows displays the name of the selected item in the
  scope pane, with nodes separated with backslashes.
- Menu:

```
File:
    New\tCtrl+N
    Open...\tCtrl+O
    Save\tCtrl+S
    Save As...
    SEPARATOR
    Add/Remove Snap-in...\tCtrl+M
    Options...
    SEPARATOR
    (list of recently opened files, see Registry. If there isn't any recent
    files stored, just display a greyed out menu item "Recent File")
    SEPARATOR
    Exit
    
Help:
    Help Topics
    TechCenter Web Site
    SEPARATOR
    About Microsoft Management Console...
    (About (Selected Snap-in))
```

- Toolbar:

```
Back (<-)
Forward (->)
SEPARATOR
Show/Hide Console Tree (Screen with arrow pointing to the left (squeezing the sidebar))
SEPARATOR
Export List (Paper with arrow pointing to the right)
SEPARATOR
Help (Paper with Question Mark)
Show/Hide Action Pane


Registry:

Stores recently opened files and some settings (I don't think it actually stores
anything) in HKCU:

```reg
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\Software\Microsoft\Microsoft Management Console]

[HKEY_CURRENT_USER\Software\Microsoft\Microsoft Management Console\Recent File List]
"File1"="C:\\Windows\\system32\\certlm.msc"
"File2"="C:\\Windows\\System32\\virtmgmt.msc"
"File3"="C:\\Windows\\system32\\printmanagement.msc"
"File4"="C:\\Windows\\system32\\dsa.msc"

[HKEY_CURRENT_USER\Software\Microsoft\Microsoft Management Console\Settings]

```