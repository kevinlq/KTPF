# KTPF
KTPF is KToolPanelFrame

简单封装了下一个二级面板功能控件，代码使用QWidget实现，面板数据通过JSON 进行配置，每个按钮执行的方法通过反射统一处理，对外预留自定义处理接口，
详细可以看实现过程

## 效果

![](/screen/Panel_Example.gif)


## 使用方法

### 01 直接实例化对象，然后添加到你的布局中即可

```
    m_pToolPane = new(std::nothrow) ToolPanel(this);
    m_pToolPane->setMinimumSize(30, this->height());
    m_pToolPane->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    QHBoxLayout* pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pToolPane);
    pHLayout->addStretch();
    pHLayout->setSpacing(0);
    pHLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pHLayout);
```

### 02 设置面板按钮点击执行方法对象

```
m_pToolPane->setRunFunObj(new(std::nothrow) ButtonFun);
```

在该对象中，添加自定义的按钮所要执行的方法

```
public Q_SLOTS:
    bool newFile(const QString &strParam);
    bool saveFile(const QString &strParam);
```

若果发现控制台输出这样类似的错误

```
QMetaObject::invokeMethod: No such method ButtonFun::xxx(const QString)
QMetaObject::invokeMethod: No such method ButtonFun::xxx(const QString)
```

那么就是你没有编写对应的函数方法，补充添加上即可

### 03 初始化面板数据

面板数据采用 `JSON`格式

```
    QFile file(QString("%1/ToolPanelData.json").arg(LIB_DIR_PATH));

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file fail.." << file.errorString();
    }
    else
    {
        QByteArray baData = file.readAll();
        file.close();

        bool bResult = m_pToolPane->initPanelData(QString(baData));

        qDebug() << "load result " << bResult;
    }
```

示例demo JSON 数据如下

``` JSON
[
    {
        "buttonID": 0,
        "buttonName": "btn0",
        "buttonTip": "btn0",
		"buttonIcon": ":/images/category_core.png",
        "buttonFun": "newFile",
		"buttonEnable": true,
        "param": "",
        "second": []
    },
    {
        "buttonID": 1,
        "buttonName": "btn1",
        "buttonTip": "btn1",
		"buttonIcon": ":/images/mode_debug.png",
        "buttonFun": "saveFile",
		"buttonEnable": true,
        "param": "",
        "second": [
            {
                "buttonID": 0,
                "buttonName": "sec0",
                "buttonTip": "sec0",
				"buttonIcon": ":/images/mode_Design.png",
                "buttonFun": "savePng0",
				"buttonEnable": true,
                "param": "",
                "second": [
                ]
            }
        ]
    }
]
```

**注意**

- 如果某个按钮有 二级面板,则只需要在 `second`字段中中嵌套配置即可；
- 按钮 `buttonID` 可以随意，但是要始终唯一，二级按钮的 `buttonID`可以和以及面板的按钮`buttonID`一样；
- 每个按钮对应的参数个数为字符串，如果需要则填写，不需要给空即可，这个参数会在调用具体函数的时候传递；


### 感谢

如果对你有用，欢迎 `star` 和 `fork`


