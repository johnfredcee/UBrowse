# UBrowse
Unreal Engine Plugin. Currently supports **Unreal Engine 5.3**
##### For browsing a graph of the low/level package/object structure and `UPROPERTY` fields of live objects in the engine.

### Features:
#### Detailed `UObject` Information
  - Reveals extensive details about individual `UObject`s, including low-level flags crucial for debugging.
  - Highlights structure among components, showcasing relationships such as Owner/Owned relationships and connections among Blueprints (`GeneratedClass`/`GeneratedBy`).
#### Useful Exploration Tool
  - Serves as a valuable tool for exploring the low-level structure of the engine.
  - Provides a unique and comprehensive view of the engine, offering insights which are challenging to capture through conventional debugging methods.
  - Enables seamless navigation among `UObject`s and `UClass`es in a hypertext-style format.
#### Easy to Access
  - Available on the context menu, world details menu, and toolbar.

![Screenshot](Docs/UBROWSE2.png)

[Quick explanatory video](https://www.youtube.com/watch?v=sM78rZ1116s)

Based in part on the [Object Browser](https://github.com/ue4plugins/ObjectBrowser) plugin by [Nick Darnell](https://github.com/nickdarnell).
