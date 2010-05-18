#ifndef _PRINT_XML_H_
#define _PRINT_XML_H_

void print_xml_open_tag(const char *tag, FILE * fp, int spaces);
void print_xml_open_tag_with_units(const char *tag, const char *units, FILE *fp, int spaces); 
void print_xml_close_tag(const char * tag, FILE * fp, int spaces);

void print_xml_header(void);
void print_xml_footer(void);
void print_xml_record_data(void *data, struct fit_record_data *record, int nelms, FILE *f, int spaces);

#endif /* _PRINT_XML_H_ */
