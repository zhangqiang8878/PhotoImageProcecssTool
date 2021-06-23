/****************************************************************************
** Meta object code from reading C++ file 'imageprocesstools.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imageprocesstools.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imageprocesstools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_getWordThread_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_getWordThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_getWordThread_t qt_meta_stringdata_getWordThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "getWordThread"
QT_MOC_LITERAL(1, 14, 13), // "processFinish"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "getWordThread\0processFinish\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_getWordThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void getWordThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        getWordThread *_t = static_cast<getWordThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processFinish(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (getWordThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&getWordThread::processFinish)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject getWordThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_getWordThread.data,
      qt_meta_data_getWordThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *getWordThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *getWordThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_getWordThread.stringdata0))
        return static_cast<void*>(const_cast< getWordThread*>(this));
    return QThread::qt_metacast(_clname);
}

int getWordThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void getWordThread::processFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_ImageProcessTools_t {
    QByteArrayData data[12];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageProcessTools_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageProcessTools_t qt_meta_stringdata_ImageProcessTools = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ImageProcessTools"
QT_MOC_LITERAL(1, 18, 9), // "loadImage"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 12), // "openFileName"
QT_MOC_LITERAL(4, 42, 9), // "saveImage"
QT_MOC_LITERAL(5, 52, 12), // "rmBackground"
QT_MOC_LITERAL(6, 65, 13), // "returnProcess"
QT_MOC_LITERAL(7, 79, 10), // "mergeImage"
QT_MOC_LITERAL(8, 90, 11), // "rotateImage"
QT_MOC_LITERAL(9, 102, 12), // "getImageText"
QT_MOC_LITERAL(10, 115, 10), // "transImage"
QT_MOC_LITERAL(11, 126, 19) // "processDialogFinish"

    },
    "ImageProcessTools\0loadImage\0\0openFileName\0"
    "saveImage\0rmBackground\0returnProcess\0"
    "mergeImage\0rotateImage\0getImageText\0"
    "transImage\0processDialogFinish"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageProcessTools[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x08 /* Private */,
       1,    0,   67,    2, 0x28 /* Private | MethodCloned */,
       4,    0,   68,    2, 0x08 /* Private */,
       5,    0,   69,    2, 0x08 /* Private */,
       6,    0,   70,    2, 0x08 /* Private */,
       7,    0,   71,    2, 0x08 /* Private */,
       8,    0,   72,    2, 0x08 /* Private */,
       9,    0,   73,    2, 0x08 /* Private */,
      10,    0,   74,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ImageProcessTools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImageProcessTools *_t = static_cast<ImageProcessTools *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->loadImage(); break;
        case 2: _t->saveImage(); break;
        case 3: _t->rmBackground(); break;
        case 4: _t->returnProcess(); break;
        case 5: _t->mergeImage(); break;
        case 6: _t->rotateImage(); break;
        case 7: _t->getImageText(); break;
        case 8: _t->transImage(); break;
        case 9: _t->processDialogFinish(); break;
        default: ;
        }
    }
}

const QMetaObject ImageProcessTools::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ImageProcessTools.data,
      qt_meta_data_ImageProcessTools,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ImageProcessTools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageProcessTools::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ImageProcessTools.stringdata0))
        return static_cast<void*>(const_cast< ImageProcessTools*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ImageProcessTools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
