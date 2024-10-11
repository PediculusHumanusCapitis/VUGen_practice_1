Action()
{
	int i;
	int count;

	lr_start_transaction("UC2_SearchTicket");
	
	GoToHomePage();

	lr_start_transaction("go_to_flights");
	
	web_reg_find("Text=Find Flight",LAST);
	
	
	web_revert_auto_header("Sec-Fetch-User");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	lr_think_time(27);
	
	web_reg_save_param_ex("ParamName=NotDefoiltCity",
	                      "LB=<option value=\"",
	                      "RB=\">",
	                      "Ordinal=ALL",
	                      LAST);
	
	web_reg_save_param_ex("ParamName=DefoiltCity",
	                      "LB=\"selected\" value=\"",
	                      "RB=\">",
	                      "Ordinal=ALL",
	                      LAST);
	
	
	web_image("Search Flights Button", 
		"Alt=Search Flights Button", 
		"Snapshot=t5.inf", 
		LAST);


	lr_end_transaction("go_to_flights",LR_AUTO);

	lr_start_transaction("find_flight");
	
	
	lr_save_string(lr_paramarr_idx("DefoiltCity", 1), "AllCities");

	count = lr_paramarr_len("NotDefoiltCity");
	
	for (i = 1; i <= count; i++) {
		char* currentCity = lr_paramarr_idx("NotDefoiltCity", i);
    	if (strstr(lr_eval_string("{AllCities}"), currentCity) == NULL) {
            lr_param_sprintf("AllCities", "%s,%s", lr_eval_string("{AllCities}"), currentCity);
        }
	}
	lr_save_param_regexp(lr_eval_string("{AllCities}"),
	                     strlen(lr_eval_string("{AllCities}")),
	                     "Ordinal=All",
	                     "ResultParam=CityArray",
	                     LAST);

	lr_save_string(lr_paramarr_random("CityArray"),"DepartureCity");
	
	lr_save_string(lr_paramarr_random("CityArray"), "ArrivalrCity");
	
	while(strcmp(lr_eval_string("{DepartureCity}"), lr_eval_string("{ArrivalrCity}")) == 0){
		lr_save_string(lr_paramarr_random("CityArray"), "ArrivalrCity");
	}

	

	web_add_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(52);
	

	web_reg_find("Text=departing from <B>{DepartureCity}</B> to <B>{ArrivalrCity}</B>",LAST);
	
	web_submit_data("reservations.pl", 
		"Action=http://localhost:1080/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/reservations.pl?page=welcome", 
		"Snapshot=t6.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=advanceDiscount", "Value=0", ENDITEM, 
		"Name=depart", "Value={DepartureCity}", ENDITEM, 
		"Name=departDate", "Value={DepartDate}", ENDITEM, 
		"Name=arrive", "Value={ArrivalrCity}", ENDITEM, 
		"Name=returnDate", "Value={ReturnDate}", ENDITEM, 
		"Name=numPassengers", "Value={NumPass}", ENDITEM, 
		"Name=seatPref", "Value={SeatPref}", ENDITEM, 
		"Name=seatType", "Value={SeatType}", ENDITEM, 
		"Name=findFlights.x", "Value=61", ENDITEM, 
		"Name=findFlights.y", "Value=10", ENDITEM, 
		"Name=.cgifields", "Value=roundtrip", ENDITEM, 
		"Name=.cgifields", "Value=seatType", ENDITEM, 
		"Name=.cgifields", "Value=seatPref", ENDITEM, 
		LAST);

	lr_end_transaction("find_flight",LR_AUTO);

	Logout();

	lr_end_transaction("UC2_SearchTicket",LR_AUTO);

	return 0;
}