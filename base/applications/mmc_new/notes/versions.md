# MMC 3.0 (Windows 11 23H2)

## mmcndmgr.dll

### Exports

- CreateExecutivePlatform
- DllCanUnloadNow
- DllGetClassObject
- DllRegisterServer
- DllUnregisterServer

### In registry

| CLSID                                    | ProgID                          | CLSID                              |
|------------------------------------------|---------------------------------|------------------------------------|
| `{0B5A7836-4C16-4560-90B2-0F5DAF6D6D1B}` | FormHost.FormHost               | FormHost Class                     |
| `{2D11CF10-4FE0-45B2-88DF-6FFBF92BE9AB}` | MMC.WaitDialog                  | WaitDialog                         |
| `{3C5F432A-EF40-4669-9974-9671D4FC2E12}` | NODEMGR.MMCProtocol             | MMC Plugable Internet Protocol     |
| `{43136EB5-D36C-11CF-ADBC-00AA00A80033}` | NODEMGR.NodeInitObject          | NodeInit 1.0 Object                |
| `{44F9A03B-A3EC-4F3B-9364-08E0007F21DF}` | Control.TaskSymbol              | TaskSymbol Class                   |
| `{4A65D267-1539-4BD1-921D-1C49B3E58EB7}` | MMC.SnapInFailureReporter       | SnapInFailureReporter              |
| `{5491AB67-AFEB-48B1-B8DF-B2D63810EF40}` |                                 | PSFactoryBuffer                    |
| `{7F1899DA-62A6-11D0-A2C6-00C04FD909DD}` | NODEMGR.ScopeTreeObject         | ScopeTree 1.0 Object               |
| `{80F94176-FCCC-11D2-B991-00C04F8ECD78}` | MessageView.MessageView         | MessageView Class                  |
| `{ADE6444B-C91F-4E37-92A4-5BB430A33340}` | NODEMGR.AppEventsDHTMLConnector | AppEventsDHTMLConnector 1.0 Object |
| `{B0395DA5-6A15-4E44-9F36-9A9DC7A2F341}` | MMC.IconControl                 | MMC IconControl class              |
| `{B3FD5602-EB0F-415E-9F32-75DA391D6BF9}` | MMC.ExecutivePlatform           | ExecutivePlatform                  |
| `{B708457E-DB61-4C55-A92F-0D4B5E9B1224}` | NODEMGR.MMCViewExt              | MMCViewExt 1.0 Object              |
| `{C96401CC-0E17-11D3-885B-00C04F72C717}` | Snapins.FolderSnapin            | Folder                             |
| `{C96401CF-0E17-11D3-885B-00C04F72C717}` | Snapins.OCXSnapin               | OCX                                |
| `{C96401D1-0E17-11D3-885B-00C04F72C717}` | Snapins.HTMLSnapin              | HTML                               |
| `{D6FEDB1D-CF21-4BD9-AF3B-C5468E9C6684}` | NODEMGR.MMCVersionInfo          | MMCVersionInfo 1.0 Object          |
| `{F0285374-DFF1-11D3-B433-00C04F8ECD78}` | ConsolePower.ConsolePower       | ConsolePower Class                 |
| `{F1E752C3-FD72-11D0-AEF6-00C04FB6DD2C}` | NODEMGR.MMCDocConfig            | DocConfig 1.0 Object               |
| `{F7A4F1DA-96C3-4BCF-BEB3-1D9FFDE89EE9}` | NODEMGR.ComCacheCleanup         | ComCacheCleanup 1.0 Object         |

| LibID                                    | Name                     |
|------------------------------------------|--------------------------|
| `{43136EB0-D36C-11CF-ADBC-00AA00A80033}` | NodeMgr 1.0 Type Library |


# MMC 1.0 (NT 4.0)

## mmcndmgr.dll

### Exports

Standard COM stuff.

- DllCanUnloadNow
- DllGetClassObject
- DllRegisterServer
- DllUnregisterServer

### In registry

| CLSID                                    | ProgID                  | CLSID Desc.          |
|------------------------------------------|-------------------------|----------------------|
| `{43136EB5-D36C-11CF-ADBC-00AA00A80033}` | NODEMGR.NodeInitObject  | NodeInit 1.0 Object  |
| `{7F1899DA-62A6-11D0-A2C6-00C04FD909DD}` | NODEMGR.ScopeTreeObject | ScopeTree 1.0 Object |
| `{F1E752C3-FD72-11D0-AEF6-00C04FB6DD2C}` | NODEMGR.MMCDocConfig    | NodeInit 1.0 Object  |

Computer\HKEY_CLASSES_ROOT\CLSID\{C96401CC-0E17-11D3-885B-00C04F72C717}\InprocServer32