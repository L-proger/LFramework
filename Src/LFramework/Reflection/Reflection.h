#include "../Macro/ForEach.h"

#define COMMA_LAST(x) x
#define COMMA_NOT_LAST(x) x ,

#define JOIN_IMPL(last, x) COMMA_##last(x)

#define JOIN2(...) UNPACK(FOR_EACH_L(JOIN_IMPL, __VA_ARGS__))
#define JOIN(...) UNPACK( JOIN2 (__VA_ARGS__) )

template<typename T>
struct Reflect {

};


#define GEN_FIELD2(type_, name_) type_ name_;
#define GEN_FIELD(field_) GEN_FIELD2 field_

#define GEN_FIELD_GETTER2(id_, type_, name_)  if constexpr(ID == FieldsCount - 1 - id_) return object. name_; 
#define GEN_FIELD_GETTER1(id_, ...) UNPACK(GEN_FIELD_GETTER2(id_, __VA_ARGS__))
#define GEN_FIELD_GETTER(id_, ...) GEN_FIELD_GETTER1(id_, UNPACK __VA_ARGS__ )


#define DEFINE_STRUCT_RTTI(structName_, publicFields_) \
template<> struct Reflect<structName_> { \
static const char* Name() {return #structName_; } \
	static constexpr size_t FieldsCount = ARGS_CNT(UNPACK publicFields_) ; \
	template<size_t ID>	static auto& getField(structName_& object){ \
		FOR_EACH_N(GEN_FIELD_GETTER, UNPACK publicFields_) ; \
	} \
};
#define DEFINE_STRUCT_TYPE(structName_, publicFields_) struct structName_ {  FOR_EACH(GEN_FIELD, UNPACK publicFields_)  };


#define DEFINE_STRUCT(structName_, publicFields_)  \
	DEFINE_STRUCT_TYPE(structName_, publicFields_) \
	DEFINE_STRUCT_RTTI(structName_, publicFields_) 



#define UNPACK_TEMPLATE_PARAMS1(last_, type_, name_) COMMA_##last_(type_ name_)
#define UNPACK_TEMPLATE_PARAM1(last, ...) UNPACK( UNPACK_TEMPLATE_PARAMS1(last, __VA_ARGS__ ))
#define UNPACK_TEMPLATE_PARAM(last, x) UNPACK_TEMPLATE_PARAM1(last, UNPACK x)


#define UNPACK_TEMPLATE_ARGS1(last_, type_, name_) COMMA_##last_(name_)
#define UNPACK_TEMPLATE_ARG1(last, ...) UNPACK( UNPACK_TEMPLATE_ARGS1(last, __VA_ARGS__ ))
#define UNPACK_TEMPLATE_ARG(last, x) UNPACK_TEMPLATE_ARG1(last, UNPACK x)

#define UNPACK_TEMPLATE_PARAMS(...) UNPACK (FOR_EACH_L(UNPACK_TEMPLATE_PARAM, __VA_ARGS__) )



#define UNPACK_TEMPLATE_ARGS(...) UNPACK (FOR_EACH_L(UNPACK_TEMPLATE_ARG, __VA_ARGS__) )


#define DEFINE_TEMPLATE_STRUCT_RTTI(templateParams_, structName_, publicFields_) \
template< UNPACK_TEMPLATE_PARAMS templateParams_ > struct Reflect<structName_< UNPACK_TEMPLATE_ARGS templateParams_ > > { \
static const char* Name() {return #structName_; } \
	static constexpr size_t FieldsCount = ARGS_CNT(UNPACK publicFields_) ; \
	template<size_t ID>	static auto& getField(structName_ < UNPACK_TEMPLATE_ARGS templateParams_ > & object){ \
		FOR_EACH_N(GEN_FIELD_GETTER, UNPACK publicFields_) ; \
	} \
};

#define DEFINE_TEMPLATE_STRUCT_TYPE(templateParams_, structName_, publicFields_) template< UNPACK_TEMPLATE_PARAMS templateParams_ > struct structName_ { FOR_EACH(GEN_FIELD, UNPACK publicFields_) };

#define DEFINE_TEMPLATE_STRUCT(templateParams_, structName_, publicFields_)  \
	DEFINE_TEMPLATE_STRUCT_TYPE(templateParams_, structName_, publicFields_) \
	DEFINE_TEMPLATE_STRUCT_RTTI(templateParams_, structName_, publicFields_) 