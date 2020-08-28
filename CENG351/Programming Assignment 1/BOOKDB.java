package ceng.ceng351.bookdb;

import ceng.ceng351.bookdb.QueryResult.ResultQ1;
import ceng.ceng351.bookdb.QueryResult.ResultQ2;
import ceng.ceng351.bookdb.QueryResult.ResultQ3;
import ceng.ceng351.bookdb.QueryResult.ResultQ4;
import ceng.ceng351.bookdb.QueryResult.ResultQ5;
import ceng.ceng351.bookdb.QueryResult.ResultQ6;
import ceng.ceng351.bookdb.QueryResult.ResultQ7;
import ceng.ceng351.bookdb.QueryResult.ResultQ8;

import java.util.List;
import java.util.ArrayList;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.*;

public class BOOKDB implements IBOOKDB {

	private static String user = "2237881";
	private static String password = "f1119246";
	private static String host = "144.122.71.65";
	private static String database = "db2237881";
	private static int port = 8084;

	private Connection connection = null;

	/**
	 * Place your initialization code inside if required.
	 *
	 * <p>
	 * This function will be called before all other operations. If your
	 * implementation need initialization , necessary operations should be done
	 * inside this function. For example, you can set your connection to the
	 * database server inside this function.
	 */

	public void initialize() {
		String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database;
		//url = "jdbc:mysql://144.122.71.65:8084/db2237881"

		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			this.connection = DriverManager.getConnection(url, this.user, this.password);
		} catch (SQLException | ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Should create the necessary tables when called.
	 *
	 * @return the number of tables that are created successfully.
	 */

	@Override
	public int createTables() {
		int count = 0;

		String q1 = "create table if not exists author( " +
				"author_id int, " +
				"author_name varchar(60), " +
				"primary key(author_id))";

		String q2 = "create table if not exists publisher( " +
				"publisher_id int, " +
				"publisher_name varchar(50), " +
				"primary key(publisher_id))";

		String q3 = "create table if not exists book(" +
				"isbn char(13), " +
				"book_name varchar(120), " +
				"publisher_id int, " +
				"first_publish_year char(4), " +
				"page_count int, " +
				"category varchar(25), " +
				"rating float, " +
				"primary key(isbn), " +
				"foreign key(publisher_id) references publisher(publisher_id) " +
				"on delete cascade on update cascade)";

		String q4 = "create table if not exists author_of(" +
				"isbn char(13), " +
				"author_id int, " +
				"primary key (isbn, author_id), " +
				"foreign key (isbn) references book(isbn) " +
				"on delete cascade on update cascade, " +
				"foreign key(author_id) references author(author_id) " +
				"on delete cascade on update cascade)";

		String q5 = "create table if not exists phw1( " +
				"isbn char(13), " +
				"book_name varchar(120), " +
				"rating float, " +
				"primary key(isbn))";

		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q1);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q2);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}

		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q3);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q4);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q5);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}

		return count;
	}

	/**
	 * Should drop the tables if exists when called.
	 *
	 * @return the number of tables are dropped successfully.
	 */

	@Override
	public int dropTables() {
		int res;
		int count = 0;
		String q1 = "Drop table if exists author_of";
		String q2 = "Drop table if exists author";
		String q3 = "Drop table if exists book";
		String q4 = "Drop table if exists publisher";
		String q5 = "Drop table if exists phw1";

		try {
			Statement stat = this.connection.createStatement();
			res = stat.executeUpdate(q1);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			res = stat.executeUpdate(q2);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			res = stat.executeUpdate(q3);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			res = stat.executeUpdate(q4);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		try {
			Statement stat = this.connection.createStatement();
			res = stat.executeUpdate(q5);
			count++;
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return count;
	}

	/**
	 * Should insert an array of Author into the database.
	 *
	 * @return Number of rows inserted successfully.
	 */

	@Override
	public int insertAuthor(Author[] authors) {
		int count = 0;
		for (int i = 0; i < authors.length; i++) {
			Author tmp = authors[i];
			String q = "insert into author values (" +
					Integer.toString(tmp.getAuthor_id()) + ", \"" +
					tmp.getAuthor_name() + "\")";
			try {
				Statement stat = this.connection.createStatement();
				stat.executeUpdate(q);
				count++;
				stat.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return count;
	}

	/**
	 * Should insert an array of Book into the database.
	 *
	 * @return Number of rows inserted successfully.
	 */

	@Override
	public int insertBook(Book[] books) {
		int count = 0;
		for (int i = 0; i < books.length; i++) {
			Book tmp = books[i];
			String q = "insert into book values ('" +
					tmp.getIsbn() + "', \"" +
					tmp.getBook_name() + "\", " +
					Integer.toString(tmp.getPublisher_id()) + ", '" +
					tmp.getFirst_publish_year() + "', " +
					Integer.toString(tmp.getPage_count()) + ", \"" +
					tmp.getCategory() + "\", " +
					Double.toString(tmp.getRating()) + ")";
			try {
				Statement stat = this.connection.createStatement();
				stat.executeUpdate(q);
				count++;
				stat.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}

		return count;
	}

	/**
	 * Should insert an array of Publisher into the database.
	 *
	 * @return Number of rows inserted successfully.
	 */

	@Override
	public int insertPublisher(Publisher[] publishers) {
		int count = 0;
		for (int i = 0; i < publishers.length; i++) {
			Publisher tmp = publishers[i];
			String q = "insert into publisher values (" +
					Integer.toString(tmp.getPublisher_id()) + ",\"" +
					tmp.getPublisher_name() + "\")";
			try {
				Statement stat = this.connection.createStatement();
				stat.executeUpdate(q);
				count++;
				stat.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return count;
	}


	/**
	 * Should insert an array of Author_of into the database.
	 *
	 * @return Number of rows inserted successfully.
	 */

	@Override
	public int insertAuthor_of(Author_of[] author_ofs) {
		int count = 0;
		for (int i = 0; i < author_ofs.length; i++) {
			Author_of tmp = author_ofs[i];
			String q = "insert into author_of values ('" +
					tmp.getIsbn() + "', " +
					Integer.toString(tmp.getAuthor_id()) + ")";
			try {
				Statement stat = this.connection.createStatement();
				stat.executeUpdate(q);
				count++;
				stat.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return count;
	}

	/**
	 * Should return isbn, first_publish_year, page_count and publisher_name of
	 * the books which have the maximum number of pages.
	 *
	 * @return QueryResult.ResultQ1[]
	 */

	@Override
	public QueryResult.ResultQ1[] functionQ1() {
		String q = "select b.isbn, b.first_publish_year, b.page_count, p.publisher_name " +
				"from book b, publisher p where b.publisher_id = p.publisher_id and b.page_count = (select max(b1.page_count) from book b1) " +
				"order by b.isbn";
		List<QueryResult.ResultQ1> list1 = new ArrayList<QueryResult.ResultQ1>();

		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				String isbn_val = resSet.getString("isbn");
				String fpy_val = resSet.getString("first_publish_year");
				String pn_val = resSet.getString("publisher_name");
				int pc_val = resSet.getInt("page_count");

				QueryResult.ResultQ1 tmp = new QueryResult.ResultQ1(isbn_val, fpy_val, pc_val, pn_val);
				list1.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list1.size();
		QueryResult.ResultQ1[] result = new QueryResult.ResultQ1[size];
		list1.toArray(result);
		return result;
	}

	/**
	 * For the publishers who have published books that were co-authored by both
	 * of the given authors(author1 and author2); list publisher_id(s) and average
	 * page_count(s)  of all the books these publishers have published.
	 *
	 * @param author_id1
	 * @param author_id2
	 * @return QueryResult.ResultQ2[]
	 */
	@Override
	public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2) {
		String q = "select p.publisher_id, avg(b.page_count) as avgpc " +
				"from book b, publisher p where b.publisher_id = p.publisher_id and " +
				"p.publisher_id in (select b1.publisher_id from book b1, author_of af1, author_of af2 " +
				"where af1.isbn = af2.isbn and af1.isbn = b1.isbn and af1.author_id = " + Integer.toString(author_id1) +
				" and af2.author_id = " + Integer.toString(author_id2) + ")" +
				"group by p.publisher_id";

		List<QueryResult.ResultQ2> list2 = new ArrayList<QueryResult.ResultQ2>();

		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				int pid_val = resSet.getInt("publisher_id");
				double avg_page_c_val = resSet.getDouble("avgpc");
				QueryResult.ResultQ2 tmp = new QueryResult.ResultQ2(pid_val, avg_page_c_val);
				list2.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list2.size();
		QueryResult.ResultQ2[] result = new ResultQ2[size];
		list2.toArray(result);
		return result;
	}

	/**
	 * List book_name, category and first_publish_year of the earliest
	 * published book(s) of the author(s) whose author_name is given.
	 *
	 * @param author_name
	 * @return QueryResult.ResultQ3[]
	 */

	public QueryResult.ResultQ3[] functionQ3(String author_name) {
		String q = "select b.book_name, b.category, b.first_publish_year " +
				"from author a, book b, author_of af " +
				"where a.author_name = \"" + author_name + "\" and af.author_id = a.author_id and " +
				"af.isbn = b.isbn and b.first_publish_year = (select min(b1.first_publish_year) " +
				"from book b1, author_of af2 where " +
				"af2.isbn = b1.isbn and af2.author_id = a.author_id) " +
				" order by b.book_name, b.category, b.first_publish_year";
		List<QueryResult.ResultQ3> list3 = new ArrayList<QueryResult.ResultQ3>();

		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				String book_n_val = resSet.getString("book_name");
				String category_val = resSet.getString("category");
				String fp_year_val = resSet.getString("first_publish_year");
				QueryResult.ResultQ3 tmp = new QueryResult.ResultQ3(book_n_val, category_val, fp_year_val);
				list3.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list3.size();
		QueryResult.ResultQ3[] result = new QueryResult.ResultQ3[size];
		list3.toArray(result);
		return result;
	}

	/**
	 * For publishers whose name contains at least 3 words
	 * (i.e., "Koc Universitesi Yayinlari"), have published at least 3 books
	 * and average rating of their books are greater than(>) 3;
	 * list their publisher_id(s) and distinct category(ies) they have published.
	 * PS: You may assume that each word in publisher_name is seperated by a space.
	 *
	 * @return QueryResult.ResultQ4[]
	 */
	public QueryResult.ResultQ4[] functionQ4() {
		String q = "select distinct p.publisher_id, b.category " +
				"from publisher p, book b " +
				"where p.publisher_name like '% % %' and b.publisher_id = p.publisher_id " +
				"and 2 < (select count(*) from book b where b.publisher_id = p.publisher_id) and " +
				"p.publisher_id in (select distinct b1.publisher_id " +
				"from book b1 group by b1.publisher_id " +
				"having avg(rating) > 3) " +
				"order by p.publisher_id, b.category ";  //b1.rating?
		List<QueryResult.ResultQ4> list4 = new ArrayList<QueryResult.ResultQ4>();

		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				int pid_val = resSet.getInt("publisher_id");
				String category_val = resSet.getString("category");
				QueryResult.ResultQ4 tmp = new QueryResult.ResultQ4(pid_val, category_val);
				list4.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list4.size();
		QueryResult.ResultQ4[] result = new QueryResult.ResultQ4[size];
		list4.toArray(result);
		return result;

	}

	/**
	 * List author_id and author_name of the authors who have worked with
	 * all the publishers that the given author_id has worked.
	 *
	 * @param author_id
	 * @return QueryResult.ResultQ5[]
	 */

	public QueryResult.ResultQ5[] functionQ5(int author_id) {
		String q = "select a.author_id, a.author_name " +
				"from author a " +
				"where " +
				"not exists (" +
				"select p.publisher_id from publisher p, book b, author_of af " +
				"where af.author_id = " + Integer.toString(author_id) + " and b.isbn = af.isbn and b.publisher_id = p.publisher_id and " +
				"p.publisher_id not in( " +
				"select p1.publisher_id from publisher p1, book b1, author_of af1 " +
				"where b1.publisher_id = p1.publisher_id and b1.isbn = af1.isbn and af1.author_id = a.author_id)) " +
				"order by a.author_id";

		List<QueryResult.ResultQ5> list5 = new ArrayList<QueryResult.ResultQ5>();

		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				int aid_val = resSet.getInt("author_id");
				String a_name_val = resSet.getString("author_name");
				QueryResult.ResultQ5 tmp = new QueryResult.ResultQ5(aid_val, a_name_val);
				list5.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list5.size();
		QueryResult.ResultQ5[] result = new QueryResult.ResultQ5[size];
		list5.toArray(result);
		return result;
	}

	/**
	 * List author id(s) and isbn(s) of the book(s) written by "Selective" authors.
	 * "Selective" authors are those who have worked with publishers that have
	 * published their books only.(i.e., they haven't published books of
	 * different authors)
	 *
	 * @return QueryResult.ResultQ6[]
	 */

	/*....."Selective" authors are those who have only worked with publishers that have published their books only...*/
	public QueryResult.ResultQ6[] functionQ6() {
		String q = "select a1.author_id, b1.isbn from author a1, book b1, author_of af1 " +
				"where af1.isbn = b1.isbn and af1.author_id = a1.author_id and " +
				"a1.author_id in (select a2.author_id from author a2, book b2, author_of af2 where b2.isbn = af2.isbn and af2.author_id = a2.author_id and " +
				"b2.publisher_id not in (select b.publisher_id from book b, author a, author_of af " +
				"where af.author_id = a.author_id and af.isbn = b.isbn and a.author_id <> a2.author_id))" +
				"order by a1.author_id, b1.isbn";

		List<QueryResult.ResultQ6> list6 = new ArrayList<QueryResult.ResultQ6>();

		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				int aid_val = resSet.getInt("author_id");
				String isbn_val = resSet.getString("isbn");
				QueryResult.ResultQ6 tmp = new QueryResult.ResultQ6(aid_val, isbn_val);
				list6.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list6.size();
		QueryResult.ResultQ6[] result = new QueryResult.ResultQ6[size];
		list6.toArray(result);
		return result;
	}

	/**
	 * List publisher_id and publisher_name of the publishers who have published
	 * at least 2 books in  'Roman' category and average rating of their books
	 * are more than (>) the given value.
	 *
	 * @param rating
	 * @return QueryResult.ResultQ7[]
	 */
	public QueryResult.ResultQ7[] functionQ7(double rating) {
		String q = "select p.publisher_id, p.publisher_name from publisher p " +
				"where 1 < (select count(*) from book b where b.category = \"Roman\" and p.publisher_id = b.publisher_id) and " +
				"p.publisher_id in (select b.publisher_id from book b " +
				"group by b.publisher_id " +
				"having avg(rating) > " + Double.toString(rating) + " )" +
				"order by p.publisher_id ";

		List<QueryResult.ResultQ7> list7 = new ArrayList<QueryResult.ResultQ7>();
		try {
			Statement stat = this.connection.createStatement();
			ResultSet resSet = stat.executeQuery(q);
			while (resSet.next()) {
				int pid_val = resSet.getInt("publisher_id");
				String pn_val = resSet.getString("publisher_name");
				QueryResult.ResultQ7 tmp = new QueryResult.ResultQ7(pid_val, pn_val);
				list7.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list7.size();
		QueryResult.ResultQ7[] result = new QueryResult.ResultQ7[size];
		list7.toArray(result);
		return result;
	}

	/**
	 * Some of the books  in the store have been published more than once:
	 * although they have same names(book\_name), they are published with different
	 * isbns. For each  multiple copy of these books, find the book_name(s) with the
	 * lowest rating for each book_name  and store their isbn, book_name and
	 * rating into phw1 table using a single BULK insertion query.
	 * If there exists more than 1 with the lowest rating, then store them all.
	 * After the bulk insertion operation, list isbn, book_name and rating of
	 * all rows in phw1 table.
	 *
	 * @return QueryResult.ResultQ8[]
	 */

	public QueryResult.ResultQ8[] functionQ8() {
		String q = "insert into phw1 (isbn, book_name, rating) " +
				"select publishedMoreThanOnce.isbn, publishedMoreThanOnce.book_name, publishedMoreThanOnce.rating " +
				"from (select b.isbn, b.book_name, b.rating from book b, book b1 where b1.isbn <> b.isbn and b.book_name = b1.book_name) as publishedMoreThanOnce " +
				"where publishedMoreThanOnce.rating = (select min(b.rating) from book b where b.book_name = publishedMoreThanOnce.book_name)";

		List<QueryResult.ResultQ8> list8 = new ArrayList<QueryResult.ResultQ8>();

		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q);
			ResultSet resSet = stat.executeQuery("select p.isbn, p.book_name, p.rating from phw1 p ");
			while (resSet.next()) {
				String isbn_val = resSet.getString("isbn");
				String bn_val = resSet.getString("book_name");
				Float rating_val = resSet.getFloat("rating");
				QueryResult.ResultQ8 tmp = new QueryResult.ResultQ8(isbn_val, bn_val, rating_val);
				list8.add(tmp);
			}
			stat.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		int size = list8.size();
		QueryResult.ResultQ8[] result = new QueryResult.ResultQ8[size];
		list8.toArray(result);
		return result;
	}

	/**
	 * For the books that contain the given keyword anywhere in their names,
	 * increase their ratings by one.
	 * Please note that, the maximum rating cannot be more than 5,
	 * therefore if the previous rating is greater than 4, do not update the
	 * rating of that book.
	 *
	 * @param keyword
	 * @return sum of the ratings of all books
	 */

	public double functionQ9(String keyword) {
		int rows = 0;
		double totalRating = 0;
		String q = "update book b1 " +
				"set b1.rating = b1.rating + 1 " +
				"where b1.book_name like \"%" + keyword + "%\" and b1.rating <= 4";

		try {
			Statement stat = this.connection.createStatement();
			rows = stat.executeUpdate(q);
			ResultSet resSet = stat.executeQuery("select b.rating from book b ");
			while (resSet.next()) {
				Float rating_val = resSet.getFloat("rating");
				totalRating += rating_val;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}

		return totalRating;
	}

	/**
	 * Delete publishers in publisher table who haven't published a book yet.
	 *
	 * @return count of all rows of the publisher table after delete operation.
	 */
	public int function10() {
		int count = 0;
		String q = "delete from publisher p " +
				"where p.publisher_id not in (select b.publisher_id from book b) ";

		try {
			Statement stat = this.connection.createStatement();
			stat.executeUpdate(q);
			ResultSet resSet = stat.executeQuery("select * from publisher p");
			while (resSet.next()) {
				count++;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return count;
	}

}