#pragma once
#include <AppCore/Defines.h>
#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSStringRef.h>
#include <Ultralight/String.h>
#include <functional>
#include <memory>

namespace ultralight {

///
/// Set the current JSContext.
///
/// Most JavaScriptCore C API calls require an active JavaScript execution
/// context (JSContextRef). You can get the JSContextRef for a page via
/// `View::LockJSContext()`. This context changes with each page navigation.
///
/// **Note**:
///    You MUST set a JSContext before using most of the C++ API below.
///
void AExport SetJSContext(JSContextRef ctx);

///
/// Get the current JSContext.
///
JSContextRef AExport GetJSContext();

///
/// JavaScript String wrapper that automatically manages JSStringRef lifetime
/// and provides helpful conversions.
///
class AExport JSString {
public:
  /// Create empty string
  JSString();

  /// Create from C-string
  JSString(const char* str);

  /// Create from Ultralight String
  JSString(const String& str);

  /// Take ownership of existing JSStringRef (will not increase ref-count)
  JSString(JSStringRef str);

  /// Copy constructor (will increase ref-count)
  JSString(const JSString& other);

  /// Destructor
  ~JSString();

  /// Assignment operator (will increase ref-count)
  JSString& operator=(const JSString& other);

  /// Cast to String
  operator String();

  /// Cast to underlying JSStringRef
  operator JSStringRef() const { return instance_; }

protected:
  JSStringRef instance_;
};

class JSArray;
class JSObject;
class JSFunction;

/// Tag type used with the JSValue constructor to create "Null" types
struct AExport JSValueNullTag {};

/// Tag type used with the JSValue constructor to create "Undefined" types
struct AExport JSValueUndefinedTag {};

///
/// JavaScript variant value wrapper that automatically manages JSValueRef 
/// lifetime and provides helpful conversions.
///
class AExport JSValue {
public:
  /// Create null (empty) JSValue
  JSValue();

  /// Create null JSValue explicitly
  JSValue(JSValueNullTag);

  /// Create undefined JSValue
  JSValue(JSValueUndefinedTag);

  /// Create boolean JSValue
  JSValue(bool val);

  /// Create unsigned integer JSValue (aka, Number) [will be cast to double]
  JSValue(uint32_t val);

  /// Create integer JSValue (aka, Number) [will be cast to double]
  JSValue(int32_t val);

  /// Create unsigned integer JSValue (aka, Number) [will be cast to double]
  JSValue(uint64_t val);

  /// Create integer JSValue (aka, Number) [will be cast to double]
  JSValue(int64_t val);

  /// Create double JSValue (aka, Number)
  JSValue(double val);

  /// Create string JSValue
  JSValue(const char* val);

  /// Create string JSValue
  JSValue(const String& val);

  /// Create string JSValue
  JSValue(JSString val);

  /// Create from existing JSValueRef
  JSValue(JSValueRef val);

  /// Create object JSValue
  JSValue(JSObjectRef obj);

  /// Copy constructor, a shallow copy is made, the constructed JSValue will
  /// point to the same JSValueRef.
  JSValue(const JSValue& other);

  /// Destructor
  virtual ~JSValue();

  /// A shallow copy is made, this JSValue will point to the same JSValueRef
  virtual JSValue& operator=(const JSValue& other);

  /// Whether or not the value is a JavaScript Null type.
  bool IsNull() const;

  /// Whether or not the value is a JavaScript Undefined type.
  bool IsUndefined() const;

  /// Whether or not the value is a JavaScript Boolean type.
  bool IsBoolean() const;

  /// Whether or not the value is a JavaScript Number type.
  bool IsNumber() const;

  /// Whether or not the value is a JavaScript String type.
  bool IsString() const;

  /// Whether or not the value is a JavaScript Object type.
  bool IsObject() const;

  /// Whether or not the value is a JavaScript Array type.
  bool IsArray() const;

  /// Whether or not the value is a JavaScript Function type.
  bool IsFunction() const;

  /// Get the value as a Boolean
  bool ToBoolean() const;

  /// Get the value as a Number (Double)
  double ToNumber() const;

  /// Get the value as a Number (Integer)
  int64_t ToInteger() const { return static_cast<int64_t>(ToNumber()); }

  /// Get the value as a String
  JSString ToString() const;

  /// Get the value as an Object (will debug assert if not an Object)
  JSObject ToObject() const;

  /// Get the value as an Array (will debug asset if not an Array)
  JSArray ToArray() const;

  /// Get the value as a Function (will debug asset if not a Function)
  JSFunction ToFunction() const;

  operator bool() const { return ToBoolean(); }

  operator double() const { return ToNumber(); }

  operator uint32_t() const { return static_cast<uint32_t>(ToNumber()); }

  operator int32_t() const { return static_cast<uint32_t>(ToNumber()); }

  operator uint64_t() const { return static_cast<uint64_t>(ToNumber()); }

  operator int64_t() const { return ToInteger(); }

  operator String() const { return ToString(); }

  operator JSString() const { return ToString(); }

  operator JSObject() const;

  operator JSObjectRef() const;

  operator JSArray() const;

  operator JSFunction() const;

  /// Get the underlying JSValueRef
  operator JSValueRef() const { return instance(); }

  ///
  /// Get the bound context for this JSValue (it is cached at creation).
  ///
  JSContextRef context() const { return ctx_; }

  ///
  /// Set the JSContext for this JSValue.
  ///
  /// **Note**:
  ///    JSValues created from within a JSCallback have a temporary JSContext
  ///    that is destroyed when the callback returns. You will need to "move"
  ///    any JSValues created within these callbacks to the View's main context
  ///    (call set_context() with the main context) before using them outside
  ///    the callback.
  ///
  void set_context(JSContextRef context) { ctx_ = context; }

protected:
  JSValue(JSContextRef ctx);
  JSValue(JSContextRef ctx, JSValueRef val);
  virtual JSValueRef instance() const;

  JSContextRef ctx_;
  JSValueRef instance_ = nullptr;
  friend class JSFunction;
};

/// 
/// A vector of JSValues, used for passing around arguments in JSCallback.
/// 
class AExport JSArgs {
public:
  /// Create an empty list of JavaScript arguments
  JSArgs();

  /// Create a list of JavaScript arguments using a C++ initializer list
  JSArgs(const std::initializer_list<JSValue>& values);

  /// Copy-constructor
  JSArgs(const JSArgs& other);

  /// Destructor
  ~JSArgs();

  /// Assignment operator
  JSArgs& operator=(const JSArgs& other);

  ///
  /// Access an element of the argument list by index.
  ///
  /// **Note**:
  ///    All JSValues are actually wrappers of JSValueRef instances so even
  ///    though this function doesn't return a JSValue& you are still operating
  ///    directly on the underlying JavaScript value instance.
  ///
  JSValue operator[](size_t pos);

  ///
  /// Access an element of the argument list by index. (const overload)
  ///
  /// **Note**:
  ///    All JSValues are actually wrappers of JSValueRef instances so even
  ///    though this function doesn't return a JSValue& you are still operating
  ///    directly on the underlying JavaScript value instance.
  ///
  const JSValue operator[](size_t pos) const;

  /// Whether or not the argument list is empty.
  bool empty() const;

  /// The number of elements in the argument list.
  size_t size() const;

  /// Clear the argument list.
  void clear();

  /// Add a new argument to the end of the list.
  void push_back(const JSValue& val);

  /// Remove the last item from the end of the list.
  void pop_back();

  /// Get the argument list as a C-array of JSValues
  JSValue* data();

  /// Get the argument list as a C-array of JSValues (const overload)
  const JSValue* data() const;
protected:
  void* instance_;
};

///
/// JSCallback typedef used for binding C++ callbacks to JavaScript functions.
///
/// Takes two arguments (const JSObject& thisObj, const JSArgs& args) and
/// returns nothing (void).
///   
typedef std::function<void(const JSObject&, const JSArgs&)> JSCallback;

///
/// JSCallbackWithRetval typedef used for binding C++ callbacks to JavaScript
/// functions with an optional return value.
///
/// Takes two arguments (const JSObject& thisObj, const JSArgs& args) and
/// returns a JSValue back to JavaScript.
///   
typedef std::function<JSValue(const JSObject&, const JSArgs&)> JSCallbackWithRetval;

///
/// Macro to help bind C++ member functions to a JSCallback
///
/// Usage: JSCallback callback = BindJSCallback(&MyClass::MyMemberFunction);
///
/// **Note**: Expected to run from within an instance of 'MyClass', note the
///           'this' keyword in the macro.
///
#define BindJSCallback(fn) (JSCallback)std::bind(fn, this, std::placeholders::_1, std::placeholders::_2)

///
/// Macro to help bind C++ member functions to a JSCallbackWithRetval
///
/// Usage: JSCallback callback = BindJSCallback(&MyClass::MyMemberFunction);
///
/// **Note**: Expected to run from within an instance of 'MyClass', note the
///           'this' keyword in the macro.
///
#define BindJSCallbackWithRetval(fn) (JSCallbackWithRetval)std::bind(fn, this, std::placeholders::_1, std::placeholders::_2)

/// 
/// Wrapper for JSObject property value (JSValue subclass). Allows new value assignment
/// to object property, binding C++ callbacks to object properties via function objects,
/// as well as value query via the JSValue interface.
/// 
class AExport JSPropertyValue : public JSValue {
public:
  virtual ~JSPropertyValue();

  /// Assign a new value to the property (internally calls JSObjectSetProperty)
  virtual JSPropertyValue& operator=(const JSValue& value);

  /// Bind to native C++ callback (creates a Function object that can be called from JS)
  JSPropertyValue& operator=(const JSCallback& callback);

  /// Bind to native C++ callback with return value (creates a Function object that can be called from JS)
  JSPropertyValue& operator=(const JSCallbackWithRetval& callback);

protected:
  virtual JSValueRef instance() const;
  JSPropertyValue(JSContextRef ctx, JSObjectRef proxy_obj, unsigned idx);
  JSPropertyValue(JSContextRef ctx, JSObjectRef proxy_obj, JSString idx);
  JSPropertyValue(const JSPropertyValue&) = default;
  JSPropertyValue& operator=(const JSPropertyValue&) = delete;

  JSObject* proxyObj_;
  bool using_numeric_idx_;
  unsigned numeric_idx_;
  JSString string_idx_;
  friend class JSArray;
  friend class JSObject;
};

///
/// JSArray wrapper that automatically manages lifetime and provides
/// convenient access to indices and Array functions.
/// 
class AExport JSArray {
public:
  /// Create empty Array
  JSArray();

  /// Create Array from list of JSValues
  JSArray(const std::initializer_list<JSValue>& values);

  /// Create Array from existing JSObjectRef (JavaScriptCore C API)
  JSArray(JSObjectRef array_obj);

  /// Copy constructor (shallow copy, will point to same instance)
  JSArray(const JSArray& other);

  ~JSArray();

  /// Assignment (shallow assignment, will point to same instance)
  JSArray& operator=(const JSArray& other);

  /// Get number of elements in the Array
  unsigned length();

  /// Push an element to back of Array
  void push(const JSValue& val);

  /// Find the index (location) of a certain value, will return -1 if not found
  int indexOf(const JSValue& val, int start = 0) const;

  /// Get a property by array index (numbering starts at 0)
  JSPropertyValue operator[](unsigned idx) const;

  /// Get the underlying JSObjectRef (JavaScriptCore C API)
  operator JSObjectRef() const { return instance_; }

  ///
  /// Get the bound context for this JSArray (it is cached at creation).
  ///
  JSContextRef context() const { return ctx_; }

  ///
  /// Set the JSContext for this JSArray.
  ///
  /// **Note**:
  ///    JSArrays created from within a JSCallback have a temporary JSContext
  ///    that is destroyed when the callback returns. You will need to "move"
  ///    any JSArrays created within these callbacks to the View's main context
  ///    (call set_context() with the main context) before using them outside
  ///    the callback.
  ///
  void set_context(JSContextRef context) { ctx_ = context; }

protected:
  JSArray(JSContextRef ctx, JSValueRef val);

  JSContextRef ctx_;
  JSObjectRef instance_;
  friend class JSValue;
};

///
/// JSObject wrapper that automatically manages lifetime and provides
/// convenient access to properties.
/// 
class AExport JSObject {
public:
  /// Create empty Object
  JSObject();

  /// Create from existing JSObjectRef from JavaScriptCore C API
  JSObject(JSObjectRef obj);
  
  /// Copy constructor (shallow copy, will point to same instance)
  JSObject(const JSObject& other);

  ~JSObject();

  /// Assignment (shallow assignment, will point to same instance)
  JSObject& operator=(const JSObject& other);

  /// Get a property by name
  JSPropertyValue operator[](JSString propertyName) const;

  /// Check if a property exists
  bool HasProperty(JSString propertyName) const;

  /// Remove a property
  bool DeleteProperty(JSString propertyName);

  /// Get the underlying JSObjectRef (JavaScriptCore C API)
  operator JSObjectRef() const { return instance_; }

  ///
  /// Get the bound context for this JSObject (it is cached at creation).
  ///
  JSContextRef context() const { return ctx_; }

  ///
  /// Set the JSContext for this JSObject.
  ///
  /// **Note**:
  ///    JSObjects created from within a JSCallback have a temporary JSContext
  ///    that is destroyed when the callback returns. You will need to "move"
  ///    any JSObjects created within these callbacks to the View's main context
  ///    (call set_context() with the main context) before using them outside
  ///    the callback.
  ///
  void set_context(JSContextRef context) { ctx_ = context; }

protected:
  JSObject(JSContextRef ctx, JSValueRef val);
  JSObject(JSContextRef ctx, JSObjectRef obj);

  JSContextRef ctx_;
  JSObjectRef instance_;
  friend class JSValue;
  friend class JSPropertyValue;
};

///
/// JSFunction wrapper that automatically manages lifetime and provides
/// convenient function invocation operators.
///
class AExport JSFunction {
public:
  /// Create an empty Function.
  /// NOTE: It is OKAY to create this without calling SetJSContext() first.
  JSFunction();

  /// Copy constructor (shallow copy, will point to same instance)
  JSFunction(const JSFunction& other);

  ~JSFunction();

  /// Assignment (shallow assignment, will point to same instance)
  JSFunction& operator=(const JSFunction& other);

  /// Whether or not this is a valid, callable Function object.
  bool IsValid() const;

  /// Call function (using Global Object for 'this') and return the result.
  JSValue operator()(const JSArgs& args);

  /// Call function (with explicit object for 'this') and return the result
  JSValue operator()(const JSObject& thisObject, const JSArgs& args);

  /// Get the underlying JSObjectRef (JavaScriptCore C API)
  operator JSObjectRef() const { return instance_; }

  ///
  /// Get the bound context for this JSFunction (it is cached at creation).
  ///
  JSContextRef context() const { return ctx_; }

  ///
  /// Set the JSContext for this JSFunction.
  ///
  /// **Note**:
  ///    JSFunctions created from within a JSCallback have a temporary JSContext
  ///    that is destroyed when the callback returns. You will need to "move"
  ///    any JSFunctions created within these callbacks to the View's main context
  ///    (call set_context() with the main context) before using them outside
  ///    the callback.
  ///
  void set_context(JSContextRef context) { ctx_ = context; }

protected:
  JSFunction(JSContextRef ctx, JSValueRef val);

  JSContextRef ctx_;
  JSObjectRef instance_;
  friend class JSValue;  
};

///
/// Get the Global Object for the current JSContext.
/// In JavaScript, this would be equivalent to the "window" object.
///
JSObject AExport JSGlobalObject();

///
/// Evaluate a string of JavaScript and return a result.
/// 
JSValue AExport JSEval(const JSString& str);

}  // namespace ultralight
