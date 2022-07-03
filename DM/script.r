library(knitr)
library(tidyverse)
library(tidytext)
library(tm)
library(dplyr)
library(caret)
library(ggplot2)
library(ggfortify)

data <- read.csv('./data/data.csv')
kable(head(data))

# This dataframe contains 8 variables that correspond to:

# InvoiceNo: Invoice number. Nominal, a 6-digit integral number uniquely assigned to each transaction. If this code starts with letter 'c', it indicates a cancellation.
# StockCode: Product (item) code. Nominal, a 5-digit integral number uniquely assigned to each distinct product.
# Description: Product (item) name. Nominal.
# Quantity: The quantities of each product (item) per transaction. Numeric.
# InvoiceDate: Invice Date and time. Numeric, the day and time when each transaction was generated.
# UnitPrice: Unit price. Numeric, Product price per unit in sterling.
# CustomerID: Customer number. Nominal, a 5-digit integral number uniquely assigned to each customer.
# Country: Country name. Nominal, the name of the country where each customer resides.

lapply(data, function(x) { length(which(is.na(x))) })
lapply(data, function(x) { length(which(x == "")) })
data[data == ""] <- NA

data <- data[complete.cases(data),]

data$InvoiceDate <- as.Date.character(data$InvoiceDate, "%m/%d/%Y %H:%M")
summary(data)
boxplot(data$UnitPrice[data$UnitPrice < 10], horizontal = TRUE)

ggplot(data) + geom_bar(aes(x = InvoiceDate))

data_grouped <- data %>% group_by(Country)
data_by_country <- data_grouped %>% summarise(
  TotQuantity = sum(Quantity),
  AvgUnitPrice = mean(UnitPrice)
)
data_by_country

length(count(data, Country, sort = TRUE)$n)     # no. of countries
length(count(data, StockCode, sort = TRUE)$n)   # no. of products
length(count(data, CustomerID, sort = TRUE)$n)  # no. of customers

count(data, CustomerID, InvoiceNo) # txns by customer

data[grepl("C", data$InvoiceNo), ] # cancelled orders

# drop cancellations for simplicity
data <- data[!grepl("C", data$InvoiceNo), ]
summary(data)

data[grepl("^[a-zA-Z]+", data$StockCode), ] # miscellaneous txns

# drop misc txns for simplicity
data <- data[!grepl("^[a-zA-Z]+", data$StockCode), ]
summary(data)

data$TotPrice <- data$UnitPrice * data$Quantity
View(data)

data_by_cid_txn <- data %>% group_by(CustomerID, InvoiceNo) %>% summarise(across(TotPrice, sum))
View(data_by_cid_txn)

# consider txns with a subtotal less than 4000 and greater than 0, better looking distribution
hist(data_by_cid_txn$TotPrice[data_by_cid_txn$TotPrice < 4000])
data <- data[data$InvoiceNo %in% data_by_cid_txn[data_by_cid_txn$TotPrice < 4000, ]$InvoiceNo, ]
data <- data[data$InvoiceNo %in% data_by_cid_txn[data_by_cid_txn$TotPrice > 0, ]$InvoiceNo, ]
data_by_cid_txn <- data %>% group_by(CustomerID, InvoiceNo) %>% summarise(across(TotPrice, sum))
hist((data %>% group_by(CustomerID, InvoiceNo) %>% summarise(across(TotPrice, sum)))$TotPrice)

data_by_cid_txn$TotalCategory <- cut(data_by_cid_txn$TotPrice,c(0, 50, 100, 200, 500, 1000, 10000, 50000))
levels(data_by_cid_txn$TotalCategory) = c(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

hist(as.integer(data_by_cid_txn$TotalCategory))

en_stopwords <-  data.frame(word = stopwords("en"))

temp_df <- unnest_tokens(data, word, Description)

temp_df <- dplyr::anti_join(temp_df, en_stopwords)
temp_df <- temp_df[nchar(temp_df$word) > 3, ] # consider root words with length > 3
freq_df <- temp_df %>%
  group_by(word) %>%
  summarize("freq" = n())
freq_df <- arrange(freq_df, desc(freq))
head(freq_df, 20)
ggplot(head(freq_df, 20), aes(x = word, y = freq, fill = word)) + geom_col() 


temp1_df <- temp_df %>% distinct(StockCode, UnitPrice)
temp1_df$ProductPriceCategory <- cut(temp1_df$UnitPrice, c(0, 1, 3, 5, 10, 100, 500, 1000, 5000))
dummy <- dummyVars(" ~ ProductPriceCategory", data=temp1_df)
final_df <- data.frame(predict(dummy, newdata=temp1_df))
temp1_df <- cbind(temp1_df, final_df)
temp1_df <- temp1_df %>% select(-StockCode, -ProductPriceCategory)
temp1_df <- temp1_df[complete.cases(temp1_df), ]
wssplot <- function(data, nc=15, seed=1234){
  wss <- (nrow(data)-1)*sum(apply(data,2,var))
  for (i in 2:nc){
    set.seed(seed)
    wss[i] <- sum(kmeans(data, centers=i)$withinss)}
  plot(1:nc, wss, type="b", xlab="Number of Clusters",
       ylab="Within groups sum of squares")
  wss
}
wssplot(temp1_df) # elbow method
kmean <- kmeans(temp1_df, 4)
kmean$centers

autoplot(kmean, temp1_df, frame = TRUE)  # product groups based on prices
kmean$size

temp2_df <- data_by_cid_txn %>% group_by(CustomerID) %>% summarize(
  TotalSales = sum(TotPrice),
  Min = min(TotPrice),
  Max = max(TotPrice),
  Mean = mean(TotPrice),
  Number = n()
)
temp2_df <- temp2_df %>% select(-CustomerID)
summary(temp2_df)

temp2_df$TotalSalesCategory <- cut(temp2_df$TotalSales, c(0, 10, 100, 1000, 10000, 100000))
dummy <- dummyVars(" ~ TotalSalesCategory", data=temp2_df)
final_df <- data.frame(predict(dummy, newdata=temp2_df))
temp2_df <- cbind(temp2_df, final_df)
temp2_df <- temp2_df %>% select(-TotalSalesCategory)
temp2_df <- temp2_df[complete.cases(temp2_df), ]
wssplot(temp2_df) # elbow method
kmean <- kmeans(temp2_df, 2)
kmean$centers

autoplot(kmean, temp2_df, frame = TRUE) # customer groups based on sales
kmean$size
