# UBrowse

Unreal Engine plugin for browsing a graph of the low/level package/object structure and UPROPERTY fields of live objects in the engine.

It shows a great amount of detail about individual UObjects including the low level flags which is handy for debugging; it also lets you navigate among UOBJECTS and UCLASSES hypertext style. It's a wholly different view of the engine which is difficult to capture from a debugger. It shows a bit of structure among components : Owner / Owned relationships and among Blueprints (GeneratedClass / GeneratedBy).
It's a useful tool for exploring the low level structure of the engine.

It compiles against Unreal Engine 5.5.

It installs a menu item on the context menu and the world details menu as well as the toolbar.


![Screenshot](Docs/UBROWSE2.png)

[There is a quick explanatory video here](https://www.youtube.com/watch?v=sM78rZ1116s)

It is based in part on the Object Browser plugin by [Nick Darnell] (https://github.com/ue4plugins/ObjectBrowser)
