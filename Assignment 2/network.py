import sys 									# for taking Command line input
import xml.etree.ElementTree as ET    		# module which implements simple and efficient API for parsing and creating xml data
import csv                                  # for  handling csv files
import geoip2.database                      # for finding country name from ip

tree = ET.parse(sys.argv[1])				# parsing the given xml file 
root = tree.getroot()						# Getting root  

distinct_ip_addresses=set()					# Creating the empty set

for po in root.iter('proto'):				# Iterating through each proto and finding the users who accessed the server via internet.org
	forw=0
	via=0
	for elem in po.iter():
		if elem.tag=='field' and elem.attrib["name"] =='http.x_forwarded_for':                                      # Checking whether it is accessed via proxy (or) not
			forw=1;
			found=elem.attrib["show"]

		if elem.tag=='field' and 'showname' in elem.attrib and elem.attrib["showname"]=='Via: Internet.org\\r\\n':   # Checking whether it is via internet.org proxy
			via=1;

	if (forw and via):						# if both the proxy and it should be via internet.org

		distinct_ip_addresses.add(found) 	# Adding the ip's to set (which doesn't allow duplicate ip's)

country_frequency={}   						# Creating empty Dictionary



######### Important Note:- keep the GeoLite2-Country.mmdb in the same location of this python file (i.e python file and GeoLite2-Country.mmdb file should be in same directory) ####################

with geoip2.database.Reader('./GeoLite2-Country.mmdb') as client:	# Reading the GeoLite2-Country database
	for x in distinct_ip_addresses:												
		try :
			response = client.country(x)							# Getting country name from ip
			if(response.country.name in country_frequency):			# Creating Distinct country's and their corresponding frequency
				country_frequency[response.country.name ]+=1 
			else:
				country_frequency[response.country.name ]=1
		except:
			pass  	


# For printing in terminal uncomment the below code

# for x in country_frequency.items():
# 	print(x,"\n")


############### Creates the country_users.csv file ######

with open('country_users.csv','w') as f:
    w = csv.writer(f)
    w.writerows(country_frequency.items())			# Inserting the values of dictionary as each row into csv file