//
//  CollectionUtils.h
//  MYUtilities
//
//  Created by Jens Alfke on 1/5/08.
//  Copyright 2008 Jens Alfke. All rights reserved.
//

#import <Foundation/Foundation.h>
#define _MYUTILITIES_COLLECTIONUTILS_ 1

// Collection creation conveniences:

#define $array(OBJS...)     ({id objs[]={OBJS}; \
                              [NSArray arrayWithObjects: objs count: sizeof(objs)/sizeof(id)];})
#define $marray(OBJS...)    ({id objs[]={OBJS}; \
                              [NSMutableArray arrayWithObjects: objs count: sizeof(objs)/sizeof(id)];})

#define $dict(PAIRS...)     ({_dictpair pairs[]={PAIRS}; \
                              _dictof(pairs,sizeof(pairs)/sizeof(_dictpair));})
#define $mdict(PAIRS...)    ({_dictpair pairs[]={PAIRS}; \
                              _mdictof(pairs,sizeof(pairs)/sizeof(_dictpair));})

#define $object(VAL)        ({__typeof(VAL) v=(VAL); _box(&v,@encode(__typeof(v)));})


// Apply a selector to each array element, returning an array of the results:
// (See also -[NSArray my_map:], which is more general but requires block support)
NSArray* $apply( NSArray *src, SEL selector, id defaultValue );
NSArray* $applyKeyPath( NSArray *src, NSString *keyPath, id defaultValue );


// Object conveniences:

BOOL $equal(id obj1, id obj2);      // Like -isEqual: but works even if either/both are nil

NSString* $string( const char *utf8Str );

#define $sprintf(FORMAT, ARGS... )  [NSString stringWithFormat: (FORMAT), ARGS]

#define $cast(CLASSNAME,OBJ)        ((CLASSNAME*)(_cast([CLASSNAME class],(OBJ))))
#define $castNotNil(CLASSNAME,OBJ)  ((CLASSNAME*)(_castNotNil([CLASSNAME class],(OBJ))))
#define $castIf(CLASSNAME,OBJ)      ((CLASSNAME*)(_castIf([CLASSNAME class],(OBJ))))
#define $castArrayOf(ITEMCLASSNAME,OBJ) _castArrayOf([ITEMCLASSNAME class],(OBJ))
#define $castIfArrayOf(ITEMCLASSNAME,OBJ) _castIfArrayOf([ITEMCLASSNAME class],(OBJ))

void setObj( id *var, id value );
BOOL ifSetObj( id *var, id value );
void setObjCopy( id *var, id valueToCopy );
BOOL ifSetObjCopy( id *var, id value );

static inline void setString( NSString **var, NSString *value ) {setObjCopy(var,value);}
static inline BOOL ifSetString( NSString **var, NSString *value ) {return ifSetObjCopy(var,value);}

BOOL kvSetObj( id owner, NSString *property, id *varPtr, id value );
BOOL kvSetObjCopy( id owner, NSString *property, id *varPtr, id value );
BOOL kvSetSet( id owner, NSString *property, NSMutableSet *set, NSSet *newSet );
BOOL kvAddToSet( id owner, NSString *property, NSMutableSet *set, id objToAdd );
BOOL kvRemoveFromSet( id owner, NSString *property, NSMutableSet *set, id objToRemove );


#define $true   ((NSNumber*)kCFBooleanTrue)
#define $false  ((NSNumber*)kCFBooleanFalse)
#define $null   [NSNull null]


@interface NSObject (MYUtils)
- (NSString*) my_compactDescription;
@end

@interface NSArray (MYUtils)
- (BOOL) my_containsObjectIdenticalTo: (id)object;
- (NSArray*) my_arrayByApplyingSelector: (SEL)selector;
- (NSArray*) my_arrayByApplyingSelector: (SEL)selector withObject: (id)object;
#if NS_BLOCKS_AVAILABLE
- (NSArray*) my_map: (id (^)(id obj))block;
- (NSArray*) my_filter: (int (^)(id obj))block;
#endif
@end

#if NS_BLOCKS_AVAILABLE
@interface NSEnumerator (MYUtils)
- (NSEnumerator*) my_map: (id (^)(id obj))block;
@end
#endif

@interface NSSet (MYUtils)
+ (NSSet*) my_unionOfSet: (NSSet*)set1 andSet: (NSSet*)set2;
+ (NSSet*) my_intersectionOfSet: (NSSet*)set1 andSet: (NSSet*)set2;
+ (NSSet*) my_differenceOfSet: (NSSet*)set1 andSet: (NSSet*)set2;
@end

#if NS_BLOCKS_AVAILABLE
@interface NSDictionary (MYUtils)
- (NSDictionary*) my_dictionaryByUpdatingValues: (id (^)(id key, id value))block;

@end
#endif


@interface NSData (MYUtils)
- (NSString*) my_UTF8ToString;
@end


// Compatibility declarations to allow NSArray/NSDictionary subscripting in iOS with Xcode 4.4.
// The compiler needs to see declarations of these methods to make the class subscriptable, but
// the iOS 5.1 SDK doesn't contain them on NSArray/NSDictionary. It appears it isn't necessary
// to actually implement these methods, though...
#if (defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && __IPHONE_OS_VERSION_MAX_ALLOWED < 60000)
@interface NSArray (PreiOS6)
- (id)objectAtIndexedSubscript:(NSUInteger)index;
@end
@interface NSMutableArray (PreiOS6)
- (void)setObject: (id)object atIndexedSubscript:(NSUInteger)index;
@end
@interface NSDictionary (PreiOS6)
- (id)objectForKeyedSubscript:(id)key;
@end
@interface NSMutableDictionary (PreiOS6)
- (void)setObject: (id)object forKeyedSubscript:(id)key;
@end
#endif


#ifdef GNUSTEP
#define kCFBooleanTrue  ([NSNumber numberWithBool: YES])
#define kCFBooleanFalse ([NSNumber numberWithBool: NO])
#endif



#pragma mark -
#pragma mark FOREACH:
    
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
#define foreach(VAR,ARR) for(VAR in ARR)

#else
struct foreachstate {NSArray *array; unsigned n, i;};
static inline struct foreachstate _initforeach( NSArray *arr ) {
    struct foreachstate s;
    s.array = arr;
    s.n = [arr count];
    s.i = 0;
    return s;
}
#define foreach(VAR,ARR) for( struct foreachstate _s = _initforeach((ARR)); \
                                   _s.i<_s.n && ((VAR)=[_s.array objectAtIndex: _s.i], YES); \
                                   _s.i++ )
#endif


// Internals (don't use directly)
typedef id _dictpair[2];
NSDictionary* _dictof(const _dictpair*, size_t count);
NSMutableDictionary* _mdictof(const _dictpair[], size_t count);
NSValue* _box(const void *value, const char *encoding);
id _cast(Class,id);
id _castNotNil(Class,id);
id _castIf(Class,id);
NSArray* _castArrayOf(Class,NSArray*);
NSArray* _castIfArrayOf(Class,NSArray*);
