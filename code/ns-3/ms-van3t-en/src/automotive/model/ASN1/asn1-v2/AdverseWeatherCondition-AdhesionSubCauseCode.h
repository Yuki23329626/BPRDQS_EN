/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "ASNfiles/ITS-Container_v2.asn"
 */

#ifndef	_AdverseWeatherCondition_AdhesionSubCauseCode_H_
#define	_AdverseWeatherCondition_AdhesionSubCauseCode_H_


#include "asn_application.h"

/* Including external dependencies */
#include "NativeInteger.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AdverseWeatherCondition_AdhesionSubCauseCode {
	AdverseWeatherCondition_AdhesionSubCauseCode_unavailable	= 0,
	AdverseWeatherCondition_AdhesionSubCauseCode_heavyFrostOnRoad	= 1,
	AdverseWeatherCondition_AdhesionSubCauseCode_fuelOnRoad	= 2,
	AdverseWeatherCondition_AdhesionSubCauseCode_mudOnRoad	= 3,
	AdverseWeatherCondition_AdhesionSubCauseCode_snowOnRoad	= 4,
	AdverseWeatherCondition_AdhesionSubCauseCode_iceOnRoad	= 5,
	AdverseWeatherCondition_AdhesionSubCauseCode_blackIceOnRoad	= 6,
	AdverseWeatherCondition_AdhesionSubCauseCode_oilOnRoad	= 7,
	AdverseWeatherCondition_AdhesionSubCauseCode_looseChippings	= 8,
	AdverseWeatherCondition_AdhesionSubCauseCode_instantBlackIce	= 9,
	AdverseWeatherCondition_AdhesionSubCauseCode_roadsSalted	= 10
} e_AdverseWeatherCondition_AdhesionSubCauseCode;

/* AdverseWeatherCondition-AdhesionSubCauseCode */
typedef long	 AdverseWeatherCondition_AdhesionSubCauseCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AdverseWeatherCondition_AdhesionSubCauseCode;
asn_struct_free_f AdverseWeatherCondition_AdhesionSubCauseCode_free;
asn_struct_print_f AdverseWeatherCondition_AdhesionSubCauseCode_print;
asn_constr_check_f AdverseWeatherCondition_AdhesionSubCauseCode_constraint;
ber_type_decoder_f AdverseWeatherCondition_AdhesionSubCauseCode_decode_ber;
der_type_encoder_f AdverseWeatherCondition_AdhesionSubCauseCode_encode_der;
xer_type_decoder_f AdverseWeatherCondition_AdhesionSubCauseCode_decode_xer;
xer_type_encoder_f AdverseWeatherCondition_AdhesionSubCauseCode_encode_xer;
oer_type_decoder_f AdverseWeatherCondition_AdhesionSubCauseCode_decode_oer;
oer_type_encoder_f AdverseWeatherCondition_AdhesionSubCauseCode_encode_oer;
per_type_decoder_f AdverseWeatherCondition_AdhesionSubCauseCode_decode_uper;
per_type_encoder_f AdverseWeatherCondition_AdhesionSubCauseCode_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _AdverseWeatherCondition_AdhesionSubCauseCode_H_ */
#include "asn_internal.h"
