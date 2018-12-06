# TMXBin

The purpose of this project is to provide tools to transform .tmx maps and .tsx tilesets from tiled
to a binary format .tmb and .tsb respectively, hence reducing the need of an XML parser and drastically
reducing the size of the file, as well as providing a thin layer to read binary files with this format
in your application.

## Supports

* Non-nested tile layers converter to .tsb format

## Does not support

* Nested tile layers conversion to .tsb format
* Object layers
* Image layers
* Big endian (sorry Xone/PS3)

## Memory related features

* A new allocator can be implemented in order to reserve memory for the internal structures used in the map and tileset loading. 
* A new InputStream can be also implemented to change the source where the binary data from the map comes from. This can be necessary when using your own file class, or when using a filesystem oriented library like PhysFS.

## To-Do

* Add testcases using XSTF
* Implement get_tileset_by_path which is needed for tileset management
* Support object layers, image layers and nested layers

## Thanks to

* Thorbjørn Lindeijer and all the [Tiled](https://www.mapeditor.org/) contributors
* Zeux for [pugixml](https://github.com/zeux/pugixml)

