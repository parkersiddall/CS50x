SELECT people.name FROM people 
WHERE people.id IN (SELECT stars.person_id FROM stars WHERE stars.movie_id 
IN (SELECT movies.id FROM movies WHERE year = 2004)) ORDER BY birth;